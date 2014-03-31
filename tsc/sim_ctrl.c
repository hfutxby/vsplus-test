#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>

#include "if626max.h"
#include "sim.h"
#include "tsc.h"

det_node* g_det = NULL;
int g_det_fd = 0;
static int g_exit = 0;

int us_sleep(long us)
{
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = us;
	return select(0, NULL, NULL, NULL, &tv);
}

void open_det(void)
{
	int ret;
	g_det_fd = open("det_nodes.dat", O_RDWR|O_CREAT, 0644);
	if(g_det_fd < 0){
		printf("%s\n", strerror(errno));
		return;
	}
	//g_det = (det_node*)malloc(sizeof(det_node)*DETMAX);
	if(ret = ftruncate(g_det_fd, sizeof(det_node)*DETMAX) < 0){
		printf("%s\n", strerror(errno));
	}
	g_det = mmap(NULL, sizeof(det_node)*DETMAX,  PROT_READ|PROT_WRITE, MAP_SHARED, g_det_fd, 0);
	if(g_det ==  MAP_FAILED){
		printf("%s\n", strerror(errno));
	}
	memset(g_det, 0, sizeof(det_node)*DETMAX);
	printf("sizeof:%d\n",sizeof(det_node)*DETMAX);
}

void close_det(void)
{
	close(g_det_fd);
}

int thr_det(void *arg)
{
	int step = 0, index = 0;
	struct timeval tv;
	while(!g_exit){
		index = random()%DETMAX;
		//检测到车辆到来
		//srandom(time(NULL));
		gettimeofday(&tv, NULL);
		srandom(tv.tv_sec + tv.tv_usec);
		step = random()%9+1;
		g_det[index].state = 1;
		g_det[index].sum_rising++;
		g_det[index].hold += step;
		//g_det[index].free += step;
		if((g_det[index].hold + g_det[index].free) > 10){ //只记录最近1s
			g_det[index].free = 10 - g_det[index].hold;
		}
		us_sleep(10000);

		//检测到车辆离开
		//srandom(time(NULL));
		gettimeofday(&tv, NULL);
		srandom(tv.tv_sec + tv.tv_usec);
		step = random()%9+1;
		g_det[index].state = 0;
		g_det[index].sum_falling++;
		g_det[index].free += step;
		if((g_det[index].hold + g_det[index].free) > 10){ //只记录最近1s
			g_det[index].hold = 10 - g_det[index].free;
		}
		us_sleep(10000);

		printf("index:%d, sum_rising:%d, sum_falling:%d\n", index, g_det[index].sum_rising, g_det[index].sum_falling);
	}
}

int main(void)
{
	open_det();
	pthread_t g_tid_det;
	pthread_create(&g_tid_det, NULL, thr_det, NULL);

	while(1){
		sleep(1);
	}
	close_det();
	return 0;
}
