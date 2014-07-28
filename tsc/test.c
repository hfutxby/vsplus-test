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
	int inter_green[6][3] = {
		{ 1,  2,  0},
		{ 1,  3,  0},
		{ 1,  4,  0},
		{ 2, 1, 50},
		{ 2, 3,  0},
		{ 2, 4, 50}
	};

	int i, j;
	int num = sizeof(inter_green) / (sizeof(int)*3);
	printf("num:%d\n", num);
	for(i = 0; i < num; i++){
			printf("sg1:%d, sg2:%d, v:%d\n", inter_green[i][0], inter_green[i][1], inter_green[i][2]);
		}
		printf("\n");
}

int main(void)
{
	test_array();	

	return 0;
}
