/* 跟踪记录信号灯状态变化 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#include "prg_track.h"
#include "tsc.h"
#include "drive.h"

static pthread_t g_tid_prg_track;
static int g_exit_prg_track = 0;

static prg_track* g_prg_track = NULL;
static prg_def* g_prg_def = NULL;

//对当前prg进行计时
void* thr_prg_track(void* arg)
{
	debug(3, "==>\n");
	int i;
	while(!g_exit_prg_track){
		for(i = 1; i < PRGMAX; i++){
			//debug(3, "prg:%d, stat:%d, tx:%d, tu:%d\n", i, g_prg_track[i].stat, g_prg_track[i].tx, g_prg_def[i].tu);
			if(g_prg_track[i].stat == 1){
				us_sleep(100*1000);
				g_prg_track[i].tx++;//对当前prg计时
				//debug(3, "prg:%d, tx:%d\n", i, g_prg_track[i].tx);
				if(g_prg_track[i].tx >= g_prg_def[i].tu){
					g_prg_track[i].tx = 0;
				}
			}
			else
				g_prg_track[i].tx = 0;
		}
	}
	
	debug(3, "<==\n");
}

//查看当前prg编号
int prg_track_cur(void)
{
	int i, ret = -1;
	for(i = 1; i < PRGMAX; i++){
		if(g_prg_track[i].stat == 1)
			ret = i;
	}

	return ret;
}

//初始化使用
int prg_track_cur_set(int index)
{
	g_prg_track[index].stat = 1;

	return 0;
}

//查看当前prg的tx
int prg_track_tx(void)
{
	int i, ret = -1;
	for(i = 1; i < PRGMAX; i++){
		if(g_prg_track[i].stat == 1)
			ret = (g_prg_track[i].tx * 10 + 5) / 10;//round to second
	}

	return ret; 
}

//选择下一个prg
int prg_track_next(void)
{
	int i, ret = -1, ret2;
	for(i = 0; i < PRGMAX; i++){
		if(g_prg_track[i].stat == 2)//找到下一个prg
			ret = i;
	}
	ret2 = prg_track_cur();
	//如果没找到，返回当前编号
	if(ret == -1)
		ret = ret2;
	else{
		g_prg_track[ret2].stat = 0;
		g_prg_track[ret].stat = 1;
	}
//FIXME:可能需要告诉控制器，方案已经切换
	return ret;
}

//export
//设定下一个prg，返回设定值，错误返回-1
int prg_track_next_set(int index)
{
	if(index >= PRGMAX)
		return -1;

	int i, ret = -1;
	for(i = 0; i < PRGMAX; i++){
		if(g_prg_track[i].stat == 2)//找到下一个prg
			ret = i;
	}
	//清除原先设定的下一个prg
	if(ret != -1){
		g_prg_track[ret].stat = 0;
	}
	//设定新的下一个prg
	g_prg_track[index].stat = 2;
	ret = index;

	return ret;
}


int init_prg_track(void)
{
	debug(3, "==>\n");
	int size = PRGMAX * sizeof(prg_def);
	g_prg_def = (prg_def*)malloc(size);
	if(!g_prg_def){
		debug(1, "malloc fail\n");
		return -1;
	}
	drv_prg_para(g_prg_def, size);//import
	drv_prg_para_dump(g_prg_def, size);//import

	size = PRGMAX * sizeof(prg_track);
	g_prg_track = (prg_track*)malloc(size);
	if(!g_prg_track){
		debug(1, "malloc fail\n");
		return -1;
	}
	memset(g_prg_track, 0, size);

	pthread_create(&g_tid_prg_track, NULL, thr_prg_track, NULL);
	debug(3, "<==\n");
}

void deinit_prg_track(void)
{
	debug(3, "==>\n");
	g_exit_prg_track = 1;
	if(g_tid_prg_track)
		pthread_join(g_tid_prg_track, NULL);

	if(g_prg_def)
		free(g_prg_def);

	if(g_prg_track)
		free(g_prg_track);
	debug(2, "<==\n");
}

