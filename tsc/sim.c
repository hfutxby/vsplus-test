#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>

#include "sim.h"
#include "tsc.h"
#include "if626max.h"

#define PROGMAX 10 //方案队列最大长度
prog g_prog_list[PROGMAX];
pthread_t g_tid_prog;
pthread_t g_tid_prog_tx;
pthread_t g_tid_sg;
static int g_exit = 0;
det_node* g_det = NULL;
//sg_node* g_sg = NULL;
int g_det_fd = 0;
//int g_sg_fd = 0;

extern us_sleep(long us);

/**************配时方案调度模拟*****************************/
typedef struct{
	int tu;//方案总时长
	int tx;//已运行时长
}prg;

#define PRGMAX 6 //方案数
prg g_prg[PRGMAX];
pthread_t g_tid_prg;

ctrl_data *g_ctrl;
int g_fd_ctrl;

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

/* 对prg进行计时 */
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
	open_ctrl();
	pthread_create(&g_tid_prg, NULL, thr_prg, NULL);
}

void deinit_prg(void)
{
	g_exit = 1;
	pthread_join(g_tid_prg);
	close_ctrl();
}

#if 0
/* 产生方案队列 */
int thr_prog(void* arg)
{
	int index, next, cycle_time;
	int i, ret = 0;
	struct timeval tv;
	while(!g_exit){
		gettimeofday(&tv, NULL);
		//printf("tv.tv_usec:%d\n", tv.tv_usec);
		srand(tv.tv_sec + tv.tv_usec);
		next = random()%20+1;//下一条方案到达时间
		index = random()%5+1;
//		index = random()%255;//下一条方案编号
//		ret = l_Prog_VSP(index);//只有vcb中已定义编号有效
//		if((ret != 1) && (ret != 5)){ 
//			printf("ret:%d\n", ret);
//			continue;
//		}
		cycle_time = random()%100+20;//下一条方案周期
		for(i = 0; i < PROGMAX; i++){
			if(g_prog_list[i].index == -1){
				g_prog_list[i].cycle_time = cycle_time;
				g_prog_list[i].cur_time = 0;
				g_prog_list[i].index = index;
				break;
			}
		}
		sleep(next);
	}
}

int thr_prog_tx(void* arg)
{
	while(!g_exit){
		while(g_prog_list[0].index == -1)
			us_sleep(100);//等待方案队列生成
		us_sleep(100000);//100ms
		g_prog_list[0].cur_time++;
		g_prog_list[0].cur_time %= g_prog_list[0].cycle_time;
	}
}

void lshift_prog_list(void)
{
	int i = 0;
	if(g_prog_list[1].index != -1){
		for(i = 0; i < PROGMAX-1; i++){
			g_prog_list[i].index = g_prog_list[i+1].index;
			g_prog_list[i].cycle_time = g_prog_list[i+1].cycle_time;
			g_prog_list[i].cur_time = g_prog_list[i+1].cur_time;
		}
		g_prog_list[PROGMAX-1].index = -1;
		g_prog_list[PROGMAX-1].cycle_time = 0;
		g_prog_list[PROGMAX-1].cur_time = 0;
	}
}

int sim_prog_actual(void)
{
	return 1;

	int over_count = 0;
	while(g_prog_list[0].index == -1){
		usleep(100000);
		if(over_count++ > 10){
			debug(1, "sim prog not prepared\n");
			return 0;
		}
	}

//	debug(3,"\ncur_prg:%0d,cycle_time:%0d,cur_time:%0d\nnext_prg:%0d,cycle_time:%0d\n", g_prog_list[0].index,
//			g_prog_list[0].cycle_time, (g_prog_list[0].cur_time/10)*10, g_prog_list[1].index,
//			g_prog_list[1].cycle_time);

	return g_prog_list[0].index;
}


int sim_prog_select(void)
{
	return 1;

	lshift_prog_list();
	return g_prog_list[0].index;
}

int sim_prog_tx(void)
{
	return (g_prog_list[0].cur_time/10)*10;
}

int sim_prog_tu(void)
{
	return 720;
	return ((g_prog_list[0]).cycle_time/10)*10;
}
#endif

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
	return g_prg[g_ctrl->prg_cur].tx;
}

int sim_prog_tu(void)
{
	return g_prg[g_ctrl->prg_cur].tu;
}
/***************检测器模拟**********************/
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

void sim_clr_falling(int index)
{
	g_det[index].sum_falling = 0;
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

//FIXME:和配置文件中的定义有关
int sim_det_exist(int index)
{
	return 1;	
}

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

/*********************************************************/
int sim_init(void)
{
	int i;
	for(i = 0; i < PROGMAX; i++){
		g_prog_list[i].index = -1;
		g_prog_list[i].cycle_time = 0;
		g_prog_list[i].cur_time = 0;
	}

	open_det();
	init_prg();

	//pthread_create(&g_tid_prog, NULL, thr_prog, NULL);
	//pthread_create(&g_tid_prog_tx, NULL, thr_prog_tx, NULL);

	
	return 0;
}

int sim_deinit(void)
{
	g_exit = 1;
	//pthread_join(g_tid_prog, NULL);
	//pthread_join(g_tid_prog_tx, NULL);
	//pthread_join(g_tid_sg, NULL);

	close_det();
	deinit_prg();

	return 0;
}
