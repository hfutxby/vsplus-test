/* 模拟产生检测器信号 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>

#include "if626max.h"
#include "sim.h"
#include "tsc.h"

//typedef struct {
//	int sum_rising; //上升沿计数
//	int sum_falling; //下降沿计数
//	int state; //计时状态，<100:占用中，>100:空闲中
//	int hold; //总占用计时
//	int free; //总空闲计时
//	int fault; //故障
//	int occ1;//占用率
//	int occ2;//平滑占用率
//}det_node;
/* 模拟规则
 * 当计时状态=0或=100时随机产生一次计时，更新状态值
 * 100ms内要对所有检测器完成一次扫描
 * 1、state计时-1，相对应的hold或free+1
 * 2、当hold+free=10时计算一次占用率occ1，同时更新平滑占用率occ2
 * occ1 = hold/(hold+free); 
 * if(occ1>occ2) occ2 = occ2 + f1*(occ1-occ2)
 * if(occ1<occ2) occ2 = occ2 + f2*(occ1-occ2)
 */

double g_f1 = 0.2;
double g_f2 = 0.2;
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

int thr_det(void* arg)
{
	int i;
	struct timeval tv_start, tv_end;
	struct timeval tv;
	int step;
	int us;
	while(!g_exit){
		gettimeofday(&tv_start, NULL);//较精确的将循环限制在100ms内
		for(i = 0; i < DETMAX; i++){
			if(g_det[i].fault){
				us_sleep(10);
				continue;
			}
			if((g_det[i].state == 0) || (g_det[i].state == 100)){//随机产生一次事件
				if(g_det[i].state == 0)//表示占用事件结束，产生一次下降沿
					g_det[i].sum_falling++;
				gettimeofday(&tv, NULL);
				srandom(tv.tv_sec + tv.tv_usec);
				//step = random()%9+1;//随机事件时长1-10，<=5为占用事件，>5为空闲事件
				step = random()%19+1;//随机事件时长1-20，<=5为占用事件，>5为空闲事件
				if(step <= 5){
					g_det[i].sum_rising++;
					g_det[i].state = step;
				}
				else{
					g_det[i].state = step - 5 + 100;
				}
			}
			//修改状态计时
			if(g_det[i].state < 100)
				g_det[i].hold++;
			else
				g_det[i].free++;
			g_det[i].state--;
			//修改占用率计算
			if((g_det[i].hold + g_det[i].free) >= 10){//累计1s（10次100ms）
				g_det[i].occ1 = (double)(g_det[i].hold*100)/(g_det[i].hold + g_det[i].free);
				if(g_det[i].occ1 > g_det[i].occ2)
					g_det[i].occ2 = g_det[i].occ2 + g_f1*(g_det[i].occ1 - g_det[i].occ2);
				else
					g_det[i].occ2 = g_det[i].occ2 + g_f2*(g_det[i].occ1 - g_det[i].occ2);
			}
			printf("ID:%3d, state:%3d, occ1:%3d, occ2:%3d, sum_rising:%4d, sum_falling:%4d\n", i, g_det[i].state, g_det[i].occ1, g_det[i].occ2, g_det[i].sum_rising, g_det[i].sum_falling);
		}
		gettimeofday(&tv_end, NULL);
		us = (tv_end.tv_sec - tv_start.tv_sec)*1000000 + (tv_end.tv_usec - tv_start.tv_usec);
		us_sleep(100000-us);
		//printf("time use: %dus\n", (tv_end.tv_sec - tv_start.tv_sec)*1000000 + (tv_end.tv_usec - tv_start.tv_usec));

	}
}

/* 设置检测器失效 */
int set_fault(void)
{
	int i, s = 0, n = 0;

	for(i = 0; i < DETMAX; i++){
		g_det[i].fault = 1;
	}

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

/* 使能检测器并立即产生一个上升沿  */
int set_rising(void)
{
	int s;

	int i;
	for(i = 0; i < DETMAX; i++){
		g_det[i].fault = 1;
	}

	while(1){
		printf("which detector set a rising:\n");
		scanf("%d", &s);
		if(s > DETMAX)
			printf("wrong det index\n");
		else{
			g_det[s].fault = 0;
			g_det[s].sum_rising++;
			g_det[s].state = 5;
		}
	}
}

int main(void)
{
	open_det();
	pthread_t g_tid_det;
	pthread_create(&g_tid_det, NULL, thr_det, NULL);

//	set_fault();
	set_rising();
//	while(1){
//		sleep(1);
//	}
	close_det();
	return 0;
}
