#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>

#include "sim.h"
#include "tsc.h"
#include "if626max.h"

int g_sg_fd;
sg_node* g_sg = NULL;

void open_sg(void)
{
	int ret;
	g_sg_fd = open("sg_nodes.dat", O_RDWR|O_CREAT, 0644);
	if(g_sg_fd < 0){
		printf("%s\n", strerror(errno));
		return;
	}
	if(ret = ftruncate(g_sg_fd, sizeof(sg_node)*SGMAX) < 0){
		printf("%s\n", strerror(errno));
	}
	g_sg = mmap(NULL, sizeof(sg_node)*SGMAX, PROT_READ|PROT_WRITE, MAP_SHARED, g_sg_fd, 0);
	if(g_sg == MAP_FAILED){
		printf("%s\n", strerror(errno));
	}
	//printf("sg:sizeof:%d\n", sizeof(sg_node)*SGMAX);
}

void close_sg(void)
{
	close(g_sg_fd);
	g_sg_fd = 0;
}

//typedef struct{
//	int stat; //信号灯状态，0-disable;1-amber;2-min_red;3-ex_red;4-prep;5-min_green;6-ex_green
//	int red_min; //最小红灯时间设定
//	int prep; //红绿过渡时间设定
//	int green_min; //最小绿灯时间设定
//	int amber;//绿红过渡时间设定
//	int time; //信号灯状态计时
//	int ext;//b00状态未变，b01进行红绿切换，b10进行绿红切换
//}sg_node;

void print_signal(int stat, int time)
{
	if((stat == 2) || (stat == 3)){
		printf("\033[40;31m%3d \033[0m", time);
	}
	if((stat == 5) || (stat == 6)){
		printf("\033[40;32m%3d \033[0m", time);
	}
	if((stat == 1) || (stat == 4)){
		printf("\033[40;33m%3d \033[0m", time);
	}
}

int main(void)
{
	open_sg();
	int i;
	while(1){
		//printf("\033[2J");
		//printf("\033[0;0H");
		for(i = 0; i < SGMAX; i++){
			print_signal(g_sg[i].stat, g_sg[i].time);
		}
		printf("\n\n");
		//sleep(1);
	}

	close_sg();
	return 0;
}
