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

pthread_t g_tid_prg;
int g_exit_prg = 0;

prg_track g_prg_track = NULL;
prg_def* g_prg_def = NULL;

//对当前prg进行计时
void* thr_prg(void* arg)
{
	prg_track *prg;
	prg_def* def;
	int i;
	while(!g_exit_prg){
		for(i = 0; i < PRGMAX; i++){
			prg = (prg_track*)(g_prg_track + i * sizeof(prg_track));
			def = (prg_def*)(g_prg_def + i * sizeof(prg_def));
			if(prg->stat == 1){
				us_sleep(100*1000);
				prg->tx++;//对当前prg计时
				if(prg->tx >= def->tu)
					prg->tx = 0;
			}
			else
				prg->tx = 0;
		}
	}
}

//查看当前prg编号
int prg_track_cur(void)
{
	int i, ret = -1;
	prg_track* prg = NULL;
	for(i = 0; i < PRGMAX; i++){
		prg = (prg_track*)(g_prg_track + i * sizeof(prg_track));
		if(prg->stat == 1)
			ret = i;
	}

	return ret;
}

//选择下一个prg
int prg_track_next(void)
{
	int i, ret = -1, ret2;
	prg_track* prg = NULL;
	for(i = 0; i < PRGMAX; i++){
		prg = (prg_track*)(g_prg_track + i * sizeof(prg_track));
		if(prg->stat == 2)//找到下一个prg
			ret = i;
	}
	ret2 = prg_track_cur();
	//如果没找到，返回当前编号
	if(ret == -1)
		ret = ret2
	else{
		prg = (prg_track*)(g_prg_track + ret2 * sizeof(prg_track));
		prg->stat = 0;
	}

	return ret;
}

//设定下一个prg
int prg_track_next_set(int index)
{
	int i, ret = -1;
	prg_track* prg = NULL;
	for(i = 0; i < PRGMAX; i++){
		prg = (prg_track*)(g_prg_track + i * sizeof(prg_track));
		if(prg->stat == 2)//找到下一个prg
			ret = i;
	}
	//清除原先设定的
	if(ret != -1){
		prg = (prg_track*)(g_prg_track + ret * sizeof(prg_track));
		prg->stat = 0;
	}
	//设定新的
	prg = (prg_track*)(g_prg_track + index * sizeof(prg_track));
	prg->stat = 0;


	return ret;
}


int init_prg(void)
{
	int size = PRGMAX * sizeof(prg_def);
	g_prg_def = (prg_def*)malloc(size);
	if(!g_prg_def){
		debug(1, "malloc fail\n");
		return -1;
	}

	drv_prg_para(g_prg_def, size);
	
	size = PRGMAX * sizeof(prg_track);
	g_prg_track = (prg_track*)malloc(size);
	if(!g_prg_track){
		debug(1, "malloc fail\n");
		return -1;
	}
	memset(g_prg_track, 0, size);

	pthread_create(&g_tid_prg, NULL, thr_prg, NULL);
}

void deinit_prg(void)
{
	g_exit_prg = 1;
	if(g_tid_prg)
		pthread_join(g_tid_prg, NULL);

	if(g_prg_def)
		free(g_prg_def);

	if(g_prg_track)
		free(g_prg_track);
}

