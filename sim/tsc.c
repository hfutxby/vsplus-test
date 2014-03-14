/*
 * 控制器为VSPLUS提供接口的实现部分
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>

#include "tsc.h"
#include "sim_signal.h"
#include "sim_vsplus.h"

int g_timer[MAXTIMER];//最低位做开关标志
int g_exit = 0;
pthread_t g_tid_timer;

void time_go(void)
{
	debug(1, "==>\n");
	int i;
	for(i = 0; i < MAXTIMER; i++){
		if(g_timer[i] >= MAXCOUNT){//达到最大值
			g_timer[i] = MAXCOUNT; 
		}
		else if(g_timer[i] % 2){
			g_timer[i] += 2;
		}
	}
	debug(1, "<==\n");
}

int ms_sleep(long us)
{
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = us;
	return select(0, NULL, NULL, NULL, &tv);
}

int thr_timer(void* arg)
{
	struct timeval tv1, tv2;
	int us = 100000, diff;
	while(!g_exit){
		gettimeofday(&tv1, NULL);
		time_go();
		gettimeofday(&tv2, NULL);
		diff = (tv2.tv_sec-tv1.tv_sec)*1000000+(tv2.tv_usec-tv1.tv_usec);
		//printf("time_go:%dus\n", diff);
		ms_sleep(us-diff);
	}
}

int _timer(int func, int index, int count)
{
	debug(1, "==>\n");
	int ret = 0;
	int i = 0;
	if((index >= 0) && (index <= MAXTIMER)){
		switch(func){
			case 1://read
				ret = g_timer[index]>>1;
				break;
			case 2://load and start
				g_timer[index] = (count<<1) + 1;
				break;
			case 3://clear
				g_timer[index] = 1;
				break;
			case 4://stop and clear
				g_timer[index] = 0;
				break;
			case 5://stop
				g_timer[index] &= 0xfffffffe;
				break;
			case 6://test
				debug(2, "test\n");
				for(i = 0; i < MAXTIMER; i++){
					printf("timer[%d],%s,count=%d\n", i, (g_timer[i]%2) ? "running" : "stopped", g_timer[i]>>1);
				}
				break;
		}
	}

	debug(1, "<==\n");
	return ret;
}

void pr_info(void)
{
	printf("input args:func index count\n");
	printf("func 1:read\n2:load and start\n3:clear\n4:stop and clear\n5:stop\n6:show all status\n");
	printf("index 0~%d\n", MAXTIMER-1);
	printf("count 0~32767\n");
}

int init_timers(void)
{
	debug(1, "==>\n");
	int i;
	for(i = 0; i < MAXTIMER; i++)
		g_timer[i] = 0;
	pthread_create(&g_tid_timer, NULL, thr_timer, NULL);

	debug(1, "<==\n");
	return 0;
}

int deinit_timers(void)
{
	debug(1, "==>\n");
	g_exit = 1;
	pthread_join(g_tid_timer, NULL);

	debug(1, "<==\n");
	return 0;
}

int program_actual(void)
{
	return signal_program_actual();
}

int program_selected(void)
{
	return signal_program_selected();
}

int init_tsc(void)
{
	init_timers(); 

	return 0;
}

int deinit_tsc(void)
{
	deinit_timers();

	return 0;
}
