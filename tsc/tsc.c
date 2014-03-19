#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "tsc.h"
#include "if626bas.h"
#include "if626max.h"

void* g_mem[3];
FILE* g_vcb_file = NULL;
#define VCB_FILE "para.vcb"

int g_timer[MAXTIMER];//最低位做开关标志
int g_exit = 0;
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
		g_vcb_file = fopen(VCB_FILE, "r+");
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

/* FIXME:sim */
int tsc_prog_actual(void)
{
	return sim_prog_actual();
}

/* FIXME:sim */
int tsc_prog_select(void)
{
	return sim_prog_select();
}

/* FIXME:sim */
int tsc_prog_tx(void)
{
	return sim_prog_tx();	
}

/* FIXME:sim */
int tsc_prog_tu(void)
{
	return sim_prog_tu();
}

int tsc_init()
{
	init_timers();	
	sim_init();

	return 0;
}

int tsc_deinit()
{
	deinit_timers();
	sim_deinit();

	return 0;
}
