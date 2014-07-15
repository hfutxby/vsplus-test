#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>

#include "tsc.h"
#include "if626max.h"

extern int us_sleep(long us);

static int g_test_timer = 0;
static pthread_t g_tid_test_timer;

void* thr_test_timer(void* arg)
{
	while(!g_test_timer){
		tsc_timer(6, 0, 0);
		sleep(1);
	}
}

int test_timer(void)
{
	int ret;
	ret = init_timers();
	if(ret == -1){
		printf("call init_timers() error\n");
		return -1;
	}

	//pthread_create(&g_tid_test_timer, NULL, thr_test_timer, NULL);

	int func = 6, index = 1, count = 0;
	while(!g_test_timer){
		//printf("func index count\n");
		//scanf("%d %d %d\n", &func, &index, &count);
		//ret = tsc_timer(func, index, count);
		ret = tsc_timer(2, 1, 0);
		printf("ret=%d\n", ret);
		sleep(1);
		tsc_timer(6, 0, 0);
		sleep(100);
	}

	deinit_timers();
}

int test_array(void)
{
	int inter_green[16][16] = {
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0, 50},
		{ 0,  0,  0, 40,  0, 50, 50, 40,  0, 60,  60, 30, 30,  0,  0,  0},
		{ 0,  0,  0, 50, 50,  0, 40, 30, 50,  0,   0, 40, 30,  0,  0,  0},
		{ 0, 50, 50,  0, 40, 50,  0, 50, 40,  0,  60,  0,  0,  0,  0,  0},
		{ 0,  0, 50, 50,  0,  0, 40, 60, 60, 50,   0, 30, 30,  0,  0,  0},
		{ 0, 50,  0, 50,  0,  0, 60,  0,  0, 30,  60, 30, 30,  0,  0,  0},
		{ 0, 50, 50,  0, 50, 50,  0, 70, 60, 60,  40, 40, 30,  0,  0,  0},
		{ 0,100, 70,100, 90,  0, 90,  0,  0,  0,   0,  0,  0,  0,  0,  0},
		{ 0,  0,100,110,100,  0, 90,  0,  0,  0,   0, 70, 70,  0,  0,  0},
		{ 0,110,  0,  0,120, 90,110,  0,  0,  0,   0,  0,  0,  0,  0,  0},
		{ 0, 80,  0, 70,  0, 70, 90,  0,  0,  0,   0,  0,  0,  0,  0,  0},
		{ 0, 90, 70,  0, 80, 90,100,  0, 30,  0,   0,  0,  0,  0,  0,  0},
		{ 0, 50, 80,  0, 70, 60, 60,  0, 80,  0,   0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0, 50},
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0, 50},
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0, 50, 50,  0}
		};

	int i, j;
	for(i = 0; i < 16; i++){
		for(j = 0; j < 16; j++){
			printf("%4d ", inter_green[i][j]);
		}
		printf("\n");
	}

}

int main(void)
{
	test_array();	

	return 0;
}
