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
 * 1、state计时-1，相对应的hold或free +1
 * 2、当hold+free=10时计算一次占用率occ1，同时更新平滑占用率occ2
 * occ1 = hold/(hold+free); 
 * if(occ1>occ2) occ2 = occ2 + f1*(occ1-occ2)
 * if(occ1<occ2) occ2 = occ2 + f2*(occ1-occ2)
 */

double g_f1 = 0.2;//占用率上升折算因子
double g_f2 = 0.2;//下降因子
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

int bsleep(long block)
{
	us_sleep(block*100*1000);
}

/* 打开检测器模拟信息文件 */
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

/* 检测器模拟信息维护 */
int thr_det(void* arg)
{
	int i;
	struct timeval tv_start, tv_end;
	struct timeval tv;
	int step;
	int us;
	while(!g_exit){
		gettimeofday(&tv_start, NULL);//较精确的将循环限制在100ms内
		//for(i = 0; i < DETMAX; i++){
		for(i = 0; i < 16; i++){
			if(g_det[i].fault){//处于fault状态的检测器不产生信号
				us_sleep(10);
				continue;
			}
			if((g_det[i].state == 0) || (g_det[i].state == 100)){//随机产生一次事件
				if(g_det[i].state == 0){//表示占用事件结束，产生一次下降沿
					g_det[i].sum_falling++;
					g_det[i].net = 0;
				}
				gettimeofday(&tv, NULL);
				srandom(tv.tv_sec + tv.tv_usec);
				step = random()%19+1;//随机事件时长1-10，<=5为占用事件，>5为空闲事件
				//step = random()%19+1;//随机事件时长1-20，<=5为占用事件，>5为空闲事件
				if(step <= 5){
					g_det[i].sum_rising++;
					g_det[i].gross = 0;
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
			g_det[i].net++;
			g_det[i].gross++;
			//修改占用率计算
			if((g_det[i].hold + g_det[i].free) >= 10){//累计1s（10次100ms）
				g_det[i].occ1 = (double)(g_det[i].hold*100)/(g_det[i].hold + g_det[i].free);
				if(g_det[i].occ1 > g_det[i].occ2)
					g_det[i].occ2 = g_det[i].occ2 + g_f1*(g_det[i].occ1 - g_det[i].occ2);
				else
					g_det[i].occ2 = g_det[i].occ2 + g_f2*(g_det[i].occ1 - g_det[i].occ2);
			}
			printf("ID:%3d, state:%3d, occ1:%3d, occ2:%3d, sum_rising:%3d, sum_falling:%3d, net:%3d, gross:%3d\n", i, g_det[i].state, g_det[i].occ1, g_det[i].occ2, g_det[i].sum_rising, g_det[i].sum_falling, g_det[i].net, g_det[i].gross);
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

/* 使检测器立即产生一个上升沿  */
int set_rising(void)
{
	int s;

	int i;
//	for(i = 0; i < DETMAX; i++){
//		g_det[i].fault = 1;
//	}

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

int set_a_rising_0(void)
{
	int s;
	int i;

	while(1){
		printf("which detector set a rising:\n");
		scanf("%d", &s);
		if(s > DETMAX)
			printf("wrong det index\n");
		else{
			g_det[s].fault = 0;
			g_det[s].sum_rising++;
			g_det[s].state = 5;
			i = s;
			printf("ID:%3d, state:%3d, occ1:%3d, occ2:%3d, sum_rising:%3d, sum_falling:%3d, net:%3d, gross:%3d\n", i, g_det[i].state, g_det[i].occ1, g_det[i].occ2, g_det[i].sum_rising, g_det[i].sum_falling, g_det[i].net, g_det[i].gross);
			us_sleep(5*100*1000);
			g_det[s].sum_falling++;
			g_det[s].state = 100;
		}
	}
}

//给脉冲型检测器一个触发信号
int set_a_rising(int det)
{
	printf("%s: ID:%d\n", __func__, det);
	int i;
	int t = 45;//退出时间间隔

	g_det[det].fault = 0;
	g_det[det].sum_rising++;
	g_det[det].state = 5;
	printf("generate a rising slope to trigger\n");
	i = det;
	//printf("ID:%3d, state:%3d, occ1:%3d, occ2:%3d, sum_rising:%3d, sum_falling:%3d, net:%3d, gross:%3d\n", i, g_det[i].state, g_det[i].occ1, g_det[i].occ2, g_det[i].sum_rising, g_det[i].sum_falling, g_det[i].net, g_det[i].gross);
	us_sleep(1000);
	printf("generate a cancel condition\n");
	us_sleep(t*100*1000);
	g_det[det].sum_falling++;
	g_det[det].net = 45;
	g_det[det].state = 100;
	us_sleep(1000);
	g_det[det].net = 0;//reset
	printf("\n");
}

//给占用率检测器一个触发信号
int set_a_hold(int det)
{
	printf("%s: ID:%d\n", __func__, det);
	int i;
	int t = 25;//持续占有触发

	g_det[det].fault = 0;
	g_det[det].sum_rising++;
	g_det[det].state = 5;
	g_det[det].hold = t;
	printf("generate a hold to trigger\n");
	i = det;
	//printf("ID:%3d, state:%3d, occ1:%3d, occ2:%3d, sum_rising:%3d, sum_falling:%3d, net:%3d, gross:%3d\n", i, g_det[i].state, g_det[i].occ1, g_det[i].occ2, g_det[i].sum_rising, g_det[i].sum_falling, g_det[i].net, g_det[i].gross);
	us_sleep(t*100*1000);
	printf("generate a cancel conditon\n");
	g_det[det].sum_falling++;
	g_det[det].state = 100;
	g_det[det].hold = 0;
	printf("\n");
}

int trigger_det(void)
{
	//检测器类型，
	//0：不存在
	//1：脉冲触发，net gap大于4s退出
	//2：持续占用2s触发，不占用即退出
	int type[46] = {0,
		1,2,1,1,//D1
			2,2,1,//D2
			2,2,1,1,//D3
			1,2,1,1,//D4
			2,2,1,//D5
			2,2,1,1,//D6
			1,1,1,1,//F7-F9
			1,1,1,1,1,1,//FL11-FL12
			1,1,1,1,1,1,//DR1-6
			1,2,1,//D13
			1,2,1,//D14
			1};//F15

	int det;
	while(1){
		printf("input det num: ");
		scanf("%d", &det);
		if((det < 1) && ( det > 45)){
			printf("wrong det num\n");
			continue;
		}

		if(type[det] == 1)
			set_a_rising(det);
		else if(type[det] == 2)
			set_a_hold(det);
		else
			printf("error! not defined\n");
	}
}

/* 车辆进入检测器 */
int in_det(int index)
{
	g_det[index].sum_rising++;//产生一个上升沿
	g_det[index].state = 1;//占用状态
	g_det[index].net = 0;//间隙计时清零
	g_det[index].gross = 0;
}

/* 车辆离开检测器 */
int out_det(int index)
{
	g_det[index].sum_falling++;//产生一个下降沿
	g_det[index].state = 101;//释放占用状态
}

/* 检测器数据更新 */
int go_det(int index, int time)
{
	if(g_det[index].state < 100){//占用中
		g_det[index].hold += time;//占用时间增加
		g_det[index].occ1 = (double)(g_det[index].hold)/(g_det[index].hold + g_det[index].free);
		g_det[index].occ2 = g_det[index].occ1;
		g_det[index].gross += time;//间隙时间增加
	}
	else{//空闲中
		g_det[index].free += time;//空闲时间增加
		g_det[index].occ1 = (double)(g_det[index].hold)/(g_det[index].hold + g_det[index].free);
		g_det[index].occ2 = g_det[index].occ1;
		g_det[index].gross += time;//间隙时间增加
		g_det[index].net += time;
	}
	int i = index;
	printf("ID:%3d, state:%3d, occ1:%3d, occ2:%3d, sum_rising:%3d, sum_falling:%3d, net:%3d, gross:%3d\n", i, g_det[i].state, g_det[i].occ1, g_det[i].occ2, g_det[i].sum_rising, g_det[i].sum_falling, g_det[i].net, g_det[i].gross);
}

int set_open(void)
{
	in_det(4);//D1_8
	go_det(4, 1);//通过用时1个100ms
	bsleep(1);

	out_det(4);
	go_det(4, 20);//到达下一个检测器用时5
	bsleep(20);

	in_det(3);//D1_5
	go_det(3, 1);//通过用时1
	go_det(4, 1);
	bsleep(1);

	out_det(3);
	go_det(3, 20);//到到下一个检测器用时5
	go_det(4, 20);
	bsleep(20);

	in_det(2);//D1_4
	go_det(2, 30);//通过用时10
	go_det(3, 30);
	go_det(4, 30);
	bsleep(30);

	out_det(2);
	go_det(2, 25);//到到下一个检测器用时5
	go_det(3, 25);	
	go_det(4, 25);	
	bsleep(25);

	in_det(1);//D1
	go_det(1, 1);//通过用时5
	go_det(2, 1);
	go_det(3, 1);
	go_det(4, 1);
	bsleep(1);

	out_det(1);
	go_det(1, 5);
	go_det(2, 5);
	go_det(3, 5);
	go_det(4, 5);
	bsleep(5);
}

int main(void)
{
	open_det();
//	pthread_t g_tid_det;
//	pthread_create(&g_tid_det, NULL, thr_det, NULL);

//	set_fault();
//	set_a_rising_0();
	trigger_det();
//	while(1){
//		int tmp;
//		scanf("%d", &tmp);
//		set_open();
//	}
	close_det();
	return 0;
}
