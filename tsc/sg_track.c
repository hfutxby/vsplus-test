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
int g_exit_sg_track = 0;

int g_fd_sg = 0;
sg_track* g_sg = NULL;


//打开灯组状态跟踪文件
void sg_track_open(void)
{
    int ret;
    g_fd_sg = open("sg_track.dat", O_RDWR|O_CREAT, 0644);
    if(g_fd_sg < 0){
        printf("%s\n", strerror(errno));
        return;
    }
    if(ret = ftruncate(g_fd_sg, sizeof(sg_track) * SGMAX) < 0){
        printf("%s\n", strerror(errno));
    }
    g_sg = mmap(NULL, sizeof(sg_track) * SGMAX, PROT_READ|PROT_WRITE, MAP_SHARED, g_fd_sg, 0);
    if(g_sg == MAP_FAILED){
        printf("%s\n", strerror(errno));
    }
}

//关闭灯组状态跟踪文件
void sg_track_close(void)
{
    close(g_fd_sg);
    g_fd_sg = 0;
}

//切换信号灯状态
void sg_track_switch(int sg, int stat)
{
	pthread_mutex_lock(&mutex_sg_track);
	g_sg[sg].stat = stat;
	g_sg[sg].time = 0;
	pthread_mutex_unlock(&mutex_sg_track);
}

//如果sg处于stat状态，返回此状态时间，否则返回-1
int sg_track_chk(int sg, int stat)
{
	int ret = 0;
	pthread_mutex_lock(&mutex_sg_track);
	if(g_sg[sg].stat == stat)
		ret = g_sg[sg].time;
	else
		ret = -1;
	pthread_mutex_unlock(&mutex_sg_track);

	return -1;
}

void sg_track_disable(int sg)
{
	pthread_mutex_lock(&mutex_sg_track);
	g_sg[sg].stat = 0;
	g_sg[sg].time = 0;
	pthread_mutex_unlock(&mutex_sg_track);
}

void sg_track_amber(int sg)
{	
	pthread_mutex_lock(&mutex_sg_track);
	g_sg[sg].stat = 1;
	g_sg[sg].time = 0;
	pthread_mutex_unlock(&mutex_sg_track);
}

void sg_track_red(int sg)
{
	pthread_mutex_lock(&mutex_sg_track);
	g_sg[sg].stat = 2;
	g_sg[sg].time = 0;
	pthread_mutex_unlock(&mutex_sg_track);
}

void sg_track_red_ext(int sg)
{
	pthread_mutex_lock(&mutex_sg_track);
	g_sg[sg].stat = 3;
	g_sg[sg].time = 0;
	pthread_mutex_unlock(&mutex_sg_track);
}

void sg_track_prep(int sg)
{
	pthread_mutex_lock(&mutex_sg_track);
	g_sg[sg].stat = 4;
	g_sg[sg].time = 0;
	pthread_mutex_unlock(&mutex_sg_track);
}

void sg_track_green(int sg)
{
	pthread_mutex_lock(&mutex_sg_track);
	g_sg[sg].stat = 5;
	g_sg[sg].time = 0;
	pthread_mutex_unlock(&mutex_sg_track);
}

void sg_track_green_ext(int sg)
{
	pthread_mutex_lock(&mutex_sg_track);
	g_sg[sg].stat = 6;
	g_sg[sg].time = 0;
	pthread_mutex_unlock(&mutex_sg_track);
}

//对信号灯状态进行计时
void* thr_sg_track(void* arg)
{
	long us;
	int i;
	struct timeval tv1, tv2;
	while(!g_exit_sg_track){
		gettimeofday(&tv1, NULL);
		for(i = 0; i < SGMAX; i++){
			g_sg[i].time++;
		}
		gettimeofday(&tv2, NULL);
		us = (tv2.tv_sec - tv1.tv_sec) * 1000000 + (tv2.tv_usec - tv1.tv_usec);
		us_sleep(100*1000 - us);
	}

}

int init_sg_track(void)
{
	sg_track_open();
	pthread_create(&g_tid_sg_track, NULL, thr_sg_track, NULL);
	return 0;
}

void deinit_sg_track(void)
{
	g_exit_sg_track = 1;
	pthread_join(g_tid_sg_track, NULL);
	sg_track_close();
}
