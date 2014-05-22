#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>

#include "sim.h"
#include "tsc.h"
#include "if626max.h"

static int g_exit = 0;
extern us_sleep(long us);

/************* 一般模拟参数控制 **************************/
ctrl_data *g_ctrl;
int g_fd_ctrl;

/* 一般模拟信息里包括配时方案切换的设置 */
void open_ctrl(void)
{
    int ret;
    g_fd_ctrl = open("ctrl.dat", O_RDWR|O_CREAT, 0644);
    if(g_fd_ctrl < 0){
        printf("%s\n", strerror(errno));
        return;
    }
    if(ret = ftruncate(g_fd_ctrl, sizeof(ctrl_data)) < 0){
        printf("%s\n", strerror(errno));
    }
    g_ctrl = mmap(NULL, sizeof(ctrl_data),  PROT_READ|PROT_WRITE, MAP_SHARED, g_fd_ctrl, 0);
    if(g_ctrl ==  MAP_FAILED){
        printf("%s\n", strerror(errno));
    }
    //memset(g_ctrl, 0, sizeof(ctrl_data));
    printf("sizeof:%d\n",sizeof(ctrl_data));
}

void close_ctrl(void)
{
    close(g_fd_ctrl);
}

/************* 配时方案调度模拟 **************************/
typedef struct{
	int tu;//方案总时长
	int tx;//已运行时长
}prg;

#define PRGMAX 6 //方案数
prg g_prg[PRGMAX];
pthread_t g_tid_prg;

/* 对运行prg进行计时 */
int thr_prg(void* arg)
{
	int i, us;
	struct timeval tv1, tv2;
	while(!g_exit){
		gettimeofday(&tv1, NULL);
		if((g_ctrl->prg_cur >= 0) && (g_ctrl->prg_cur < PRGMAX)){//判断当前方案号是否有效
			g_prg[g_ctrl->prg_cur].tx++;
			if(g_prg[g_ctrl->prg_cur].tx >= g_prg[g_ctrl->prg_cur].tu){//完成一个周期
				g_prg[g_ctrl->prg_cur].tx = 0;
			}
		}
		gettimeofday(&tv2, NULL);
		us = (tv2.tv_sec - tv1.tv_sec)*1000000 + (tv2.tv_usec - tv1.tv_usec);
		//printf("id:%3d tu:%3d tx:%3d\n", g_ctrl->prg_cur, g_prg[g_ctrl->prg_cur].tu, g_prg[g_ctrl->prg_cur].tx);
		us_sleep(100000 - us);//100ms完成一次计时
	}
}

void init_prg(void)
{
	int i;
	memset(g_prg, 0, sizeof(prg)*PRGMAX);
	for(i = 0; i < PRGMAX; i++){//根据XML中的数据
		g_prg[i].tu = 720;
		g_prg[i].tx = 0;
	}
	//必须在此之前调用open_ctrl();
	g_ctrl->prg_cur = 1;//默认执行方案
	pthread_create(&g_tid_prg, NULL, thr_prg, NULL);
}

void deinit_prg(void)
{
	g_exit = 1;
	pthread_join(g_tid_prg);
}

int sim_prog_actual(void)
{
	if((g_ctrl->prg_cur >= 0) && (g_ctrl->prg_cur < PRGMAX))//判断当前方案号是否有效
		return g_ctrl->prg_cur;
}

int sim_prog_select(void)
{
	if((g_ctrl->prg_next != -1) && (g_ctrl->prg_next >= 0) && (g_ctrl->prg_next < PRGMAX)){
		g_ctrl->prg_cur = g_ctrl->prg_next;
		g_ctrl->prg_next = -1;
	}
	return g_ctrl->prg_cur;
}

int sim_prog_tx(void)
{
	int ret = (g_prg[g_ctrl->prg_cur].tx+5)/10;
	return ret*10;
}

int sim_prog_tu(void)
{
	return g_prg[g_ctrl->prg_cur].tu;
}

/*************** 检测器模拟 **********************/
det_node* g_det = NULL;
int g_det_fd = 0;

/* 打开检测器模拟数据 */
void open_det(void)
{
    int ret;
    g_det_fd = open("det_nodes.dat", O_RDWR|O_CREAT, 0644);
    if(g_det_fd < 0){
        printf("%s\n", strerror(errno));
        return;
    }
    if(ret = ftruncate(g_det_fd, sizeof(det_node)*DETMAX) < 0){
        printf("%s\n", strerror(errno));
    }
    g_det = mmap(NULL, sizeof(det_node)*DETMAX,  PROT_READ|PROT_WRITE, MAP_SHARED, g_det_fd, 0);
    if(g_det ==  MAP_FAILED){
        printf("%s\n", strerror(errno));
    }
    //printf("det:sizeof:%d\n",sizeof(det_node)*DETMAX);
}

void close_det(void)
{
    close(g_det_fd);
    g_det_fd = 0;
}

int sim_sum_rising(int index)
{
	return g_det[index].sum_rising;
}

void sim_clr_rising(int index)
{
	g_det[index].sum_rising = 0;
}

int sim_sum_falling(int index)
{
	return g_det[index].sum_falling;
}

int sim_clr_falling(int index)
{
	g_det[index].sum_falling = 0;
	return 0;//g_det[index].sum_falling;
}

int sim_cur_hold(int index)
{
	return g_det[index].occ1;
}

int sim_sm_hold(int index)
{
	return g_det[index].occ2;	
}

int sim_hold_state(int index)
{
	if(g_det[index].state < 100)
		return 1;
	else
		return 0;
}

int sim_hold_time(int index)
{
	return g_det[index].hold;
}

int sim_det_fault(int index)
{
	return g_det[index].fault;
}

int sim_det_net(int index)
{
	return g_det[index].net;
}

int sim_det_gross(int index)
{
	return g_det[index].gross;
}

//FIXME:和配置文件中的定义有关
int sim_det_exist(int index)
{
	if((index >= 1) && (index <=45))
		return 1;	
	else
		return 0;
}

/************* 模拟功能初始化 ***************************/
int sim_init(void)
{
	int i;

	open_ctrl();
	open_det();
	init_prg();

	return 0;
}

int sim_deinit(void)
{
	deinit_prg();
	close_det();
	close_ctrl();

	return 0;
}
