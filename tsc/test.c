#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>

#include "tsc.h"
#include "if626max.h"

//#define SRC_FILE "para.vcb"

extern int us_sleep(long us);
//int us_sleep(int us)
//{
//	struct timeval tv;
//	tv.tv_sec = 0;
//	tv.tv_usec = us;
//	return select(0, NULL, NULL, NULL, &tv);
//}

void test_vcb(void)
{
	int ret = 0;
	ret = tsc_open_vcb();
	printf("ret=%d\n", ret);
	char buf[1024];
	ret = tsc_read_vcb(buf, 128);
	printf("ret=%d\n", ret);
	printf("%s\n", buf);
	tsc_close_vcb();
}

void test_prog(void)
{
	tsc_init();

	int ret, i = 0;
	while(1){
		i++;
		printf("tsc_prog_actual():%d\n", tsc_prog_actual());
		printf("cur_time:%ds, cycle_time:%ds\n", tsc_prog_tx()/10, tsc_prog_tu()/10);
		if((i % 10) == 0){
			printf("==>tsc_prog_select():%d\n\n", tsc_prog_select());
		}
		sleep(1);
	}

	tsc_deinit();
}

void test_det(void)
{
	open_det();
	int ret = 0, index;
	while(1){
		srandom(time(NULL));
		index = random()%DETMAX;
		ret = sim_sum_rising(index);
		printf("index:%d, sum_rising:%d", index, ret);
		ret = sim_sum_falling(index);
		printf(" sum_falling:%d\n", ret);
		sleep(1);
		sim_clr_falling(index);
	}
	close_det();
}

volatile int g_ret = -1;
static int g_exit = 0;

int func_sch1(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	srandom(tv.tv_sec + tv.tv_usec);
	int ret = random()%1500000;
	printf("into %s, sleep %dms\n", __func__, ret/1000);
	us_sleep(ret);//1.5s

	return 0;
}

int func_sch2(void)
{
	printf("switched to %s\n", __func__);
	return 0;
}


int thr_sch(void* arg)
{
	struct timeval tv1, tv2;
	while(1){
		if(g_ret == -1){//g_ret=-1时开始调用一次函数
			gettimeofday(&tv1, NULL);
			//printf("start2:%ds%dus\n", tv1.tv_sec, tv1.tv_usec);
			g_ret = func_sch1();	
			gettimeofday(&tv2, NULL);
			//printf("timediff:%dms\n\n", (tv2.tv_sec-tv1.tv_sec)*1000+(tv2.tv_usec-tv1.tv_usec)/1000);
		}
	}
}

/* 定时调用一个函数，如果这个函数没能在规定时间内返回
 * 则切换到另一个函数 */
int test_sch(void)
{
	pthread_t tid_sch;
	pthread_create(&tid_sch, NULL, thr_sch, NULL);

	struct timeval tv;
	while(1){
		g_ret = -1;
		gettimeofday(&tv, NULL);
		//printf("start1:%ds%dus\n", tv.tv_sec, tv.tv_usec);
		us_sleep(1000000);
		if(g_ret == -1){//未能在规定时间内完成调用
			func_sch2();
		}
	}
}

int test_double(void)
{
	int hold = 4, free = 6;
	int occ1 = 0, occ2 = 0;
	double f1 = 0.2, f2 = 0.2;
	occ1 = (double)(hold*100)/(hold+free);
	occ2 = occ2 + f1*(occ1-occ2);
	printf("%d, %d\n", occ1, occ2);

	return 0;
}

int main(void)
{
	test_double();	

	return 0;
}
