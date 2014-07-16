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

#include "sg_track.h"
#include "tsc.h"

pthread_mutex_t mutex_sg_track = PTHREAD_MUTEX_INITIALIZER;
pthread_t g_tid_sg_track;
static int g_exit_sg_track = 0;

static int g_fd_sg_track = 0;
static sg_track* g_sg_track = NULL;

//打开灯组状态跟踪文件
int sg_track_open(void)
{
	debug(3, "==>\n");
    int ret;
    g_fd_sg_track = open("sg_track.dat", O_RDWR|O_CREAT, 0644);
    if(g_fd_sg_track < 0){
        debug(1, "%s\n", strerror(errno));
        return -1;
    }
    if(ret = ftruncate(g_fd_sg_track, sizeof(sg_track) * SGMAX) < 0){
        debug(1, "%s\n", strerror(errno));
		return -1;
    }
    g_sg_track = mmap(NULL, sizeof(sg_track) * SGMAX, PROT_READ|PROT_WRITE, MAP_SHARED, g_fd_sg_track, 0);
    if(g_sg_track == MAP_FAILED){
        debug(1, "%s\n", strerror(errno));
		return -1;
    }

	memset(g_sg_track, 0, sizeof(sg_track) * SGMAX);

	debug(3, "<==\n");
	return 0;
}

//关闭灯组状态跟踪文件
void sg_track_close(void)
{
	int ret = -1;
	ret = munmap(g_sg_track, sizeof(sg_track) * SGMAX);
	if(ret != 0){
		debug(1, "%s\n", strerror(errno));
	}
    close(g_fd_sg_track);
    g_fd_sg_track = 0;
}

//切换信号灯状态
void sg_track_switch(int sg, int stat)
{
	pthread_mutex_lock(&mutex_sg_track);
	g_sg_track[sg].stat = stat;
	g_sg_track[sg].time = 0;
	pthread_mutex_unlock(&mutex_sg_track);
}

//如果sg处于stat状态，返回此状态时间，否则返回-1
int sg_track_chk(int sg, int stat)
{
	if(sg == 2)
		;//debug(2, "sg:%d stat: %d\n", sg, g_sg_track[sg].stat);
	int ret = 0;
	pthread_mutex_lock(&mutex_sg_track);
	if(g_sg_track[sg].stat == stat)
		ret = g_sg_track[sg].time;
	else
		ret = -1;
	pthread_mutex_unlock(&mutex_sg_track);

	return ret;
}

//设置sg处于故障状态
int sg_track_fault_set(int sg, int type)
{
	int ret = 0;
	pthread_mutex_lock(&mutex_sg_track);
	if(type)
		g_sg_track[sg].fault = 1;
	else
		g_sg_track[sg].fault = 0;
	pthread_mutex_unlock(&mutex_sg_track);

	return 0;
}

//检查sg是否处于故障状态
int sg_track_fault(int sg)
{
	return g_sg_track[sg].fault;
}

//对信号灯状态进行计时
void* thr_sg_track(void* arg)
{
	long us;
	int i;
	struct timeval tv1, tv2;
	while(!g_exit_sg_track){
		gettimeofday(&tv1, NULL);
		pthread_mutex_lock(&mutex_sg_track);
		for(i = 0; i < SGMAX; i++){
			//if(!g_sg_track[i].fault && g_sg_track[i].stat)
				g_sg_track[i].time++;
		}
		pthread_mutex_unlock(&mutex_sg_track);
		gettimeofday(&tv2, NULL);
		//debug(2, "sg:%d, stat:%d, time:%d\n", 2, g_sg_track[2].stat, g_sg_track[2].time);
		us = (tv2.tv_sec - tv1.tv_sec) * 1000 * 1000 + (tv2.tv_usec - tv1.tv_usec);
		us_sleep(100 * 1000 - us);
	}

}

int init_sg_track(void)
{
	debug(3, "==>\n");
	int ret = sg_track_open();
	if(ret != 0)
		return -1;

	pthread_create(&g_tid_sg_track, NULL, thr_sg_track, NULL);
	debug(3, "<==\n");
	return 0;
}

void deinit_sg_track(void)
{
	debug(3, "==>\n");
	g_exit_sg_track = 1;
	if(g_tid_sg_track)
		pthread_join(g_tid_sg_track, NULL);
	sg_track_close();
	debug(3, "<==\n");
}
