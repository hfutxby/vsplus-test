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
	if(ret = ftruncate(g_det_fd, sizeof(det_node)*DETMAX) < 0){
		printf("%s\n", strerror(errno));
	}
	g_det = mmap(NULL, sizeof(det_node)*DETMAX,  PROT_READ|PROT_WRITE, MAP_SHARED, g_det_fd, 0);
	if(g_det ==  MAP_FAILED){
		printf("%s\n", strerror(errno));
	}
	memset(g_det, 0, sizeof(det_node)*DETMAX);
	//printf("sizeof:%d\n",sizeof(det_node)*DETMAX);
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
		index = random()%DETMAX;//检测器编号
		//检测到车辆到来
		gettimeofday(&tv, NULL);
		srandom(tv.tv_sec + tv.tv_usec);
		step = random()%9+1;
		g_det[index].state = 1;//车辆到来
		g_det[index].sum_rising++;//产生上升沿
		g_det[index].hold += step;//过车占用时间（1-10)*100ms
		if((g_det[index].hold + g_det[index].free) > 10){ //只记录最近1s
			g_det[index].free = 10 - g_det[index].hold;
		}
		us_sleep(100000/DETMAX);

		//检测到车辆离开
		gettimeofday(&tv, NULL);
		srandom(tv.tv_sec + tv.tv_usec);
		step = random()%9+1;
		g_det[index].state = 0;//车辆离开
		g_det[index].sum_falling++;//产生下降沿
		g_det[index].free += step;//空闲时间
		if((g_det[index].hold + g_det[index].free) > 10){ //只记录最近1s
			g_det[index].hold = 10 - g_det[index].free;
		}
		us_sleep(100000/DETMAX);

//		printf("index:%d, sum_rising:%d, sum_falling:%d\n", index, g_det[index].sum_rising, g_det[index].sum_falling);
	}
}

int set_fault(void)
{
	int i, s = 0, n = 0;
	while(1){
		printf("fault det: ");
		for(i = 0; i < DETMAX; i++){
			if(g_det[i].fault)
				printf("%d ", i);
		}
		printf("\n");
		printf("1:set a det to fault\n");
		printf("2:reset a det\n");
		scanf("%d", &s);
		switch(s){
			case 1:
				printf("input 0-%d\n", DETMAX);
				scanf("%d", &n);
				if((n > 0) && (n < DETMAX))
					g_det[n].fault = 1;
				break;
			case 2:
				printf("input 0-%d\n", DETMAX);
				scanf("%d", &n);
				if((n > 0) && (n < DETMAX))
					g_det[n].fault = 0;
				break;
			default:
				printf("wrong select\n");
		}
	}
}

int main(void)
{
	open_det();
	pthread_t g_tid_det;
	pthread_create(&g_tid_det, NULL, thr_det, NULL);

	set_fault();
//	while(1){
//		sleep(1);
//	}
	close_det();
	return 0;
}
