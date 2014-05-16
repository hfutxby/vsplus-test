/* 信号灯模拟 */
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

int us_sleep(long us)
{
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = us;
	return select(0, NULL, NULL, NULL, &tv);
}

/* 打开信号灯信息记录文件 */
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

/* 打印信号灯信息 */
void print_signal(int stat, int time)
{
//	if(stat == 2) {
//		printf("\033[40;31m%4d:2 \033[0m", time);
//	}
//	if(stat == 3) {
//		printf("\033[40;31m%4d:3 \033[0m", time);
//	}
//	}if((stat == 5) || (stat == 6)){
//		printf("\033[40;32m%3d \033[0m", time);
//	}
//	if((stat == 1) || (stat == 4)){
//		printf("\033[40;33m%3d \033[0m", time);
//	}
	switch(stat){
	case 2:
		printf("\033[40;31m%4d:2 \033[0m", time);
		break;
	case 3:
		printf("\033[40;31m%4d:3 \033[0m", time);
		break;
	case 5:
		printf("\033[40;32m%4d:5 \033[0m", time);
		break;
	case 6:
		printf("\033[40;32m%4d:6 \033[0m", time);
		break;
	case 1:
		printf("\033[40;33m%4d:1 \033[0m", time);
		break;
	case 4:
		printf("\033[40;33m%4d:4 \033[0m", time);
		break;
	default:
		break;
	}
}

int main(void)
{
	open_sg();
	int i;
	while(1){
		for(i = 0; i < SGMAX; i++){
			print_signal(g_sg[i].stat, g_sg[i].time);
		}
		printf("\033[0m\r");
		us_sleep(100);
	}

	close_sg();
	return 0;
}
