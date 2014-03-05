#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>

#define MAXTIMER 3
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

void sigroutine(int signo)
{
	switch(signo){
		case SIGALRM:
			printf("SIGALRM\n");
			time_go();
			signal(SIGALRM, sigroutine);
			break;
		case SIGVTALRM:
			printf("SIGVTALRM\n");
			signal(SIGVTALRM, sigroutine);
			break;
	}
}

int thr_timer(void* arg)
{
	debug(1, "==>\n");
	struct itimerval value, ovalue, value2;
	signal(SIGALRM, sigroutine);
	signal(SIGVTALRM, sigroutine);
	value.it_value.tv_sec = 0;
	value.it_value.tv_usec = 100000;
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = 100000;
	setitimer(ITIMER_REAL, &value, &value);
	while(!g_exit){
		//sleep(1);fprintf(stderr, ".");
	}

	debug(1, "<==\n");
	pthread_exit();
}

int ms_sleep(long ms)
{
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = ms*1000;
	return select(0, NULL, NULL, NULL, &tv);
}

int thr_timer_2(void* arg)
{
	struct timeval tv1, tv2;
	int ms = 100;
	while(!g_exit){
		//gettimeofday(&tv1, NULL);
		time_go();
		//gettimeofday(&tv2, NULL);
		//printf("time_go:%dus\n", (tv2.tv_sec-tv1.tv_sec)*1000000+(tv2.tv_usec-tv1.tv_usec));
		ms_sleep(ms);
	}
}

int init_timers(void)
{
	debug(1, "==>\n");
	int i;
	for(i = 0; i < MAXTIMER; i++)
		g_timer[i] = 0;
	pthread_create(&g_tid_timer, NULL, thr_timer_2, NULL);

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

int main(void)
{
#if 1
	init_timers();

	timer(6, 0, 0);
	timer(2, 0, 0);
	sleep(2);
	timer(6, 0, 0);

	deinit_timers();
#endif
#if 0

#endif
#if 0
	struct itimerval value, ovalue, value2;
	printf("process id is %d\n", getpid());
	signal(SIGALRM, sigroutine);
	signal(SIGVTALRM, sigroutine);
	value.it_value.tv_sec = 1;
	value.it_value.tv_usec = 0;
	value.it_interval.tv_sec = 1;
	value.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &value, &value);
	//while(1);
	pthread_t tid_timer;
	pthread_create(&tid_timer, NULL, thr_timer, NULL);
	pthread_join(tid_timer, NULL);
#endif

	return 0;
}
