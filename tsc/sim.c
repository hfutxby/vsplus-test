#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>

#include "sim.h"
#include "tsc.h"
#include "if626max.h"

#define PROGMAX 10
prog g_prog_list[PROGMAX];
prog g_prog;	 
pthread_t g_tid_prog;
pthread_t g_tid_tx;
static int g_exit = 0;
det_node* g_det = NULL;
int g_det_fd = 0;

extern us_sleep(long us);

int thr_prog(void* arg)
{
	int index, next, cycle_time;
	int i;
	while(!g_exit){
		srand(time(NULL));
		next = random()%20+1;//下一条方案到达时间
		index = random()%255;//下一条方案编号
		cycle_time = random()%100+20;//下一条方案周期
		for(i = 0; i < PROGMAX; i++){
			if(g_prog_list[i].index == -1){
				g_prog_list[i].index = index;
				g_prog_list[i].cycle_time = cycle_time;
				g_prog_list[i].cur_time = 0;
				break;
			}
		}
		sleep(next);
	}
}

int thr_prog_tx(void* arg)
{
	while(!g_exit){
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
	int over_count = 0;
	while(g_prog_list[0].index == -1){
		usleep(100000);
		if(over_count > 10){
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
	lshift_prog_list();
	return g_prog_list[0].index;
}

int sim_prog_tx(void)
{
	return (g_prog_list[0].cur_time/10)*10;
}

int sim_prog_tu(void)
{
	return (g_prog_list[0]).cycle_time;
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
	return g_det[index].hold;
}

int sim_sm_hold(int index)
{
	return g_det[index].hold;//FIXME	
}

int sim_hold_state(int index)
{
	return g_det[index].state;
}

int sim_hold_time(int index)
{
	return g_det[index].hold;
}

void open_det(void)
{
	int ret;
	g_det_fd = open("det_nodes.dat", O_RDWR|O_CREAT, 0644);
	if(g_det_fd < 0){
		printf("%s\n", strerror(errno));
		return;
	}
	//g_det = (det_node*)malloc(sizeof(det_node)*DETMAX);
	if(ret = ftruncate(g_det_fd, sizeof(det_node)*DETMAX) < 0){
		printf("%s\n", strerror(errno));
	}
	g_det = mmap(NULL, sizeof(det_node)*DETMAX,  PROT_READ|PROT_WRITE, MAP_SHARED, g_det_fd, 0);
	if(g_det ==  MAP_FAILED){
		printf("%s\n", strerror(errno));
	}
	printf("sizeof:%d\n",sizeof(det_node)*DETMAX);
}

void close_det(void)
{
	close(g_det_fd);
}

int sim_init(void)
{
	int i;
	for(i = 0; i < PROGMAX; i++){
		g_prog_list[i].index = -1;
		g_prog_list[i].cycle_time = 0;
		g_prog_list[i].cur_time = 0;
	}

	pthread_create(&g_tid_prog, NULL, thr_prog, NULL);
	pthread_create(&g_tid_tx, NULL, thr_prog_tx, NULL);

	open_det();
	
	return 0;
}

int sim_deinit(void)
{
	g_exit = 1;
	pthread_join(g_tid_prog, NULL);

	close_det();

	return 0;
}
