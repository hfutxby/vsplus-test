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
sg_node* g_sg = NULL;
int g_det_fd = 0;
int g_sg_fd = 0;

extern us_sleep(long us);
int g_min_red[15] = {40,40,40,40,40,40,80,80,80,80,40,40,40,40,60};
int g_min_green[15] = {20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};

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


//打开灯组运行信息文件
void open_sg(void)
{
	int ret;
	g_sg_fd = open("sg_nodes.dat", O_RDWR|O_CREAT, 0644);
	if(g_sg_fd < 0){
		printf("%s\n", strerror(errno));
		return;
	}
	if(ret = ftruncate(g_sg_fd, sizeof(sg_node)*SGMAX) < 0){
		printf("%s\n", strerror(errno));
	}
	g_sg = mmap(NULL, sizeof(sg_node)*SGMAX, PROT_READ|PROT_WRITE, MAP_SHARED, g_sg_fd, 0);
	if(g_sg == MAP_FAILED){
		printf("%s\n", strerror(errno));
	}
	//printf("sg:sizeof:%d\n", sizeof(sg_node)*SGMAX);

	//FIXME:实际需要从XML文件中解析初始化数据
	memset(g_sg, 0, sizeof(sg_node)*SGMAX);
	int i;
	for(i = 0; i < 15; i++){
		g_sg[i].stat = 2;
		g_sg[i].red_min = g_min_red[i];
		g_sg[i].green_min = g_min_green[i];
		g_sg[i].prep = 3;
		g_sg[i].amber = 3;
		g_sg[i].time = 0;
		g_sg[i].ext = 0;
	}
}

void close_sg(void)
{
	close(g_sg_fd);
	g_sg_fd = 0;
}

//对信号灯状态进行计时
int thr_sg(void* arg)
{
	int i;
	while(!g_exit){
		for(i = 0; i < SGMAX; i++){
			g_sg[i].time++;
			switch(g_sg[i].stat){
				case 1://1-amber;2-min_red;3-ex_red;4-prep;5-min_green;6-ex_green
					if(g_sg[i].time > g_sg[i].amber){
						g_sg[i].stat++;
						g_sg[i].time = 0;
					}
					break;
				case 2:
					if(g_sg[i].time > g_sg[i].red_min){
						g_sg[i].stat++;
						g_sg[i].time = 0;
					}
					break;
				case 3://b00状态未变，b01进行红绿切换，b10进行绿红切换
					if(g_sg[i].ext == 1){
						g_sg[i].ext = 0;
						g_sg[i].stat++;
						g_sg[i].time = 0;
					}
					break;
				case 4:
					if(g_sg[i].time > g_sg[i].prep){
						g_sg[i].stat++;
						g_sg[i].time = 0;
					}
					break;
				case 5:
					if(g_sg[i].time > g_sg[i].green_min){
						g_sg[i].stat++;
						g_sg[i].time = 0;
					}
					break;
				case 6:
					if(g_sg[i].ext == 2){
						g_sg[i].ext = 0;
						g_sg[i].stat = 1;
						g_sg[i].time = 0;
					}
					break;//FIXME
			}

		}
		us_sleep(100000);
	}
}

int sim_init(void)
{
	int i;
	for(i = 0; i < PROGMAX; i++){
		g_prog_list[i].index = -1;
		g_prog_list[i].cycle_time = 0;
		g_prog_list[i].cur_time = 0;
	}

	open_det();
	open_sg();

	pthread_create(&g_tid_prog, NULL, thr_prog, NULL);
	pthread_create(&g_tid_prog_tx, NULL, thr_prog_tx, NULL);
	pthread_create(&g_tid_sg, NULL, thr_sg, NULL);

	
	return 0;
}

int sim_deinit(void)
{
	g_exit = 1;
	pthread_join(g_tid_prog, NULL);
	pthread_join(g_tid_prog_tx, NULL);
	pthread_join(g_tid_sg, NULL);

	close_det();
	close_sg();

	return 0;
}
