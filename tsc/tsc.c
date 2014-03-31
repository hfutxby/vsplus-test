#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "tsc.h"
#include "if626bas.h"
#include "if626max.h"

void* g_mem[3]; //VSPLUS内部分配用于存储参数
FILE* g_vcb_file = NULL; //配置参数vcb文件
FILE* g_vcb_back = NULL; //备份参数vcb文件
#define VCB_FILE "para.vcb" //配置参数vcb文件
#define VCB_BACK "para_bak.vcb" //备份参数vcb文件
#define _SIM_TEST_

int g_timer[MAXTIMER];//最低位做开关标志
static int g_exit = 0;
pthread_t g_tid_timer;

/* 分配存储区  */
void* tsc_alloc_mem(int size, int id)
{
	if((id > 3) || (id < 1)){
		debug(1, "id range (1-3)\n");
		return NULL;
	}
	else{
		if(g_mem[id-1] != NULL ){
			debug(2, "area %d already allocated\n", id);
			return g_mem[id-1];
		}
		else{
			g_mem[id-1] = malloc(size);
			if(g_mem[id-1] == NULL){
				debug(1, "area %d allocate fail\n", id);
				return NULL;
			}
			return g_mem[id-1];
		}
	}
}

/* 释放存储区 */
void tsc_free_mem(int id)
{
	if((id > 3) || (id < 1)){
		debug(1, "id range (1-3)\n");
	}
	else{
		if(g_mem[id-1] == NULL){
			debug(2, "area %d already freed\n", id);
		}
		free(g_mem[id-1]);
		g_mem[id-1] = NULL;
	}
}

/* 存储区地址 */
void* tsc_get_mem(int id)
{
	if((id > 3) || (id < 1)){
		debug(1, "id range (1-3)\n");
		return NULL;
	}
	else{
		if(g_mem[id-1] == NULL){
			debug(2, "area %d not allocated\n", id);
			return NULL;
		}
		debug(3, "area %d:\"%s\"\n", id, (char*)g_mem[id-1]);
		return g_mem[id-1];
	}
}

/* 打开VCB文件 */
int tsc_open_vcb(void)
{
	if(g_vcb_file != NULL){
		debug(2, "vcb file already opened\n");
		return 1;
	}
	else{
		g_vcb_file = fopen(VCB_FILE, "rb");
		if(g_vcb_file == NULL){
			debug(1, "%s\n", strerror(errno));
			return 0;
		}
		return 1;
	}
}

/* 关闭VCB文件 */
void tsc_close_vcb(void)
{
	if(g_vcb_file == NULL){
		debug(2, "vcb file not opened\n");
		return;
	}
	else{
		if(fclose(g_vcb_file) != 0){
			debug(1, "%s\n", strerror(errno));
		}
		g_vcb_file = NULL;
	}
}

/* 读VCB数据 */
int tsc_read_vcb(char *data, int size)
{
	if(g_vcb_file == NULL){
		debug(1, "vcb file not opened\n");
		return -1;
	}
	memset(data, 0, size);
	return fread(data, 1, size, g_vcb_file);

}

void time_go(void)
{
	//debug(3, "==>\n");
	int i;
	for(i = 0; i < MAXTIMER; i++){
		if(g_timer[i] >= MAXCOUNT){//达到最大值
			g_timer[i] = MAXCOUNT;
		}
		else if(g_timer[i] % 2){
			g_timer[i] += 2;
		}
	}
}

int us_sleep(long us)
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
		us_sleep(us-diff);
	}
}

/* 定时器 */
int tsc_timer(int func, int index, int count)
{
	debug(3, "==>\n");
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

	return ret;
}

int init_timers(void)
{
	debug(3, "==>\n");
	int i;
	for(i = 0; i < MAXTIMER; i++)
		g_timer[i] = 0;
	pthread_create(&g_tid_timer, NULL, thr_timer, NULL);

	debug(3, "<==\n");
	return 0;
}

int deinit_timers(void)
{
	debug(3, "==>\n");
	g_exit = 1;
	pthread_join(g_tid_timer, NULL);

	debug(3, "<==\n");
	return 0;
}

/* FIXME
 * 当前执行的配时方案 */
int tsc_prog_actual(void)
{
#ifdef _SIM_TEST_
	return sim_prog_actual();
#else

#endif
}

/* FIXME
 * 选择新的配时方案 */
int tsc_prog_select(void)
{
#ifdef _SIM_TEST_
	return sim_prog_select();
#else

#endif
}

/* FIXME
 * 当前配时方案，运行处于方案内的时间点
 */
int tsc_prog_tx(void)
{
#ifdef _SIM_TEST_
	return sim_prog_tx();	
#else

#endif
}

/* FIXME
 * 当前配时方案，总的时长
 */
int tsc_prog_tu(void)
{
#ifdef _SIM_TEST_
	return sim_prog_tu();
#else

#endif
}

/* FIXME
 * VSPLUS告知控制器某条交通流已等待很长时间，
 * 控制器判断是否切回定时配时方案
 */
void tsc_stream_waiting(int index, int time)
{
	debug(2, "stream:%d, waiting:%d\n", index, time);
}

/* FIXME
 * 读取检测器上升沿计数
 */
int tsc_sum_rising(int index)
{
#ifdef _SIM_TEST_
	return sim_sum_rising(index);
#else

#endif
}

/* FIXME
 * 清空检测器上升沿计数
 */
void tsc_clr_rising(int index)
{
#ifdef _SIM_TEST_
	sim_clr_rising(index);
#else

#endif
}

/* FIXME
 * 读取检测器下降沿计数
 */
int tsc_sum_falling(int index)
{
#ifdef _SIM_TEST_
	return sim_sum_falling(index);
#else

#endif
}

/* FIXME
 * 清空检测器下降沿计数
 */
void tsc_clr_falling(int index)
{
#ifdef _SIM_TEST_
	sim_clr_falling(index);
#else

#endif
}

/* FIXME
 * 检测器占用率，单位是percent
 */
int tsc_cur_hold(int index)
{
#ifdef _SIM_TEST_
	return sim_cur_hold(index);
#else

#endif
}

/* FIXME
 * 平滑处理后的检测器占用率，单位是percent
 */
int tsc_sm_hold(int index)
{
#ifdef _SIM_TEST_
	return sim_sm_hold(index);
#else

#endif
}

/* FIXME
 * 检测器当前占用状态
 */
int tsc_hold_state(int index)
{
#ifdef _SIM_TEST_
	return sim_hold_state(index);
#else

#endif
}

/* FIXME
 * 检测器当前已占用时间
 */
int tsc_hold_time(int index)
{
#ifdef _SIM_TEST_
	return sim_hold_time(index);
#else

#endif
}

/* FIXME
 * 测试指定的signal group是否存在
 */
int tsc_sg_exist(int index)
{
	if(index < 4)
		return 1;
	else
		return 1;//0;
}

/* FIXME
 * 检测指定的检测器是否存在
 */
int tsc_det_exist(int index)
{
	return 1;
}

/* 打开备份vcb文件 */
int tsc_open_back(void)
{
	g_vcb_back = fopen(VCB_BACK, "wb+");
	if(!g_vcb_back){
		debug(1, "open backup vcb file error\n");
		return 0;
	}

	return 1;
}

/* 写入备份vcb文件 */
int tsc_write_back(char* data, int size)
{
	if(!g_vcb_back){
		debug(1, "backup vcb file not opened yet!\n");
		return;
	}
	return fwrite(data, 1, size, g_vcb_back);
}

/* 关闭备份文件 */
void tsc_close_back(void)
{
	if(!g_vcb_back){
		debug(2, "backup vcb file not opened yet!\n");
	}
	fclose(g_vcb_back);
	g_vcb_back = 0;
}

/* FIXME
 * 判断当前是否处于vsplus动态控制中。
 * 返回0表示处于定时配时控制中
 */
int tsc_ctl_active(void)
{
	return 1;
}

/* 返回当前时间 */
int tsc_get_time(int* hour, int* min, int* sec)
{

}

int tsc_init()
{
	init_timers();	
#ifdef _SIM_TEST_
	sim_init(); //在sim.c中实现部分测试数据的产生
#endif

	return 0;
}

int tsc_deinit()
{
	deinit_timers();
#ifdef _SIM_TEST_
	sim_deinit();
#endif

	return 0;
}
