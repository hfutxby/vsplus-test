#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>

#define MAXTIMER 5
#define MAXCOUNT 32767<<1
int g_timer[MAXTIMER];//最低位做开关标志
int g_exit = 0;
pthread_t g_tid_timer;

#define d_printf(fmt, args...)	printf("\033[40;33m(%s:%d)\033[0m"fmt, __func__, __LINE__, ##args);
//level=1,track; level=2,watch;
#if DEBUG
#define debug(level, fmt, args...) \
if(DEBUG==level) \
printf("\033[40;33m(%s:%d)\033[0m"fmt, __func__, __LINE__, ##args);
#else
#define debug(level, fmt, args...)	
#endif

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

int timer(int func, int index, int count)
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
int test_timer(void)
{
	init_timers();
	int func, index, count, ret;
	while(1){
		pr_info();
		scanf("%d %d %d", &func, &index, &count);
		if(func == 1){
			ret = timer(func, index, count);
			printf("timer[%d],count=%d\n", index, ret);
		}
		else{
			ret = timer(func, index, count);
		}
	}
}

int main(void)
{
	test_timer();

#if 0
	init_timers();

	timer(6, 0, 0);
	timer(2, 0, 0);
	sleep(2);
	timer(6, 0, 0);
	usleep(300000);
	printf("read timer0 %d\n", timer(1, 0, 0));

	deinit_timers();
#endif

	return 0;
}
