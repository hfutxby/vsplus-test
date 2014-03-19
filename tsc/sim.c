#include <stdio.h>
#include <stdlib.h>

#include "sim.h"
#include "tsc.h"

#define PROGMAX 10
typedef struct{
	int index;
	int cycle_time;
	int cur_time;
}prog;
prog g_prog_list[PROGMAX];
prog g_prog;	 
pthread_t g_tid_prog;
pthread_t g_tid_tx;
static int g_exit = 0;

extern ms_sleep(long us);

int thr_prog(void* arg)
{
	int index, next, cycle_time;
	int i;
	while(!g_exit){
		srand(time(NULL));
		next = random()%20+1;
		index = random()%255;
		cycle_time = random()%100+20;
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
		ms_sleep(100000);//1s
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

//	debug(3,"cur:%0d-%0d-%0d,next:%0d-%0d\n", g_prog_list[0].index,
//			g_prog_list[0].cycle_time, (g_prog_list[0].cur_time/10)*10, g_prog_list[1].index,
//			g_prog_list[1].cycle_time);

	return g_prog_list[0].index;
}


int sim_prog_select(void)
{
	lshift_prog_list();
//	debug(3,"cur:%0d-%0d,next:%0d-%0d\n", g_prog_list[0].index,
//			g_prog_list[0].cycle_time, g_prog_list[1].index,
//			g_prog_list[1].cycle_time);
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
	
	return 0;
}

int sim_deinit(void)
{
	g_exit = 1;
	pthread_join(g_tid_prog, NULL);

	return 0;
}
