/* 独立小程序，观察信号灯状态 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>

#include "tsc.h"
#include "sg_track.h"
#include "vcb.h"

int g_sg_fd;
sg_track* g_sg = NULL;

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
	g_sg_fd = open("sg_track.dat", O_RDWR|O_CREAT, 0644);
	if(g_sg_fd < 0){
		printf("%s\n", strerror(errno));
		return;
	}
	if(ret = ftruncate(g_sg_fd, sizeof(sg_track) * SGMAX) < 0){
		printf("%s\n", strerror(errno));
	}
	g_sg = mmap(NULL, sizeof(sg_track) * SGMAX, PROT_READ|PROT_WRITE, MAP_SHARED, g_sg_fd, 0);
	if(g_sg == MAP_FAILED){
		printf("%s\n", strerror(errno));
	}
}

void close_sg(void)
{
	close(g_sg_fd);
	g_sg_fd = 0;
}

/* 打印信号灯信息 */
void print_signal(int stat, int time)
{
	switch(stat){
		case 2:
			printf("\033[40;31m%3d:2 \033[0m", time);
			break;
		case 3:
			printf("\033[40;31m%3d:3 \033[0m", time);
			break;
		case 5:
			printf("\033[40;32m%3d:5 \033[0m", time);
			break;
		case 6:
			printf("\033[40;32m%3d:6 \033[0m", time);
			break;
		case 7:
			printf("\033[40;34m%3d:7 \033[0m", time);
			break;
		case 1:
			printf("\033[40;33m%3d:1 \033[0m", time);
			break;
		case 4:
			printf("\033[40;33m%3d:4 \033[0m", time);
			break;
		case 0:
			printf("\033[0m%3d:0 ", time);
			break;	
		default:
			break;
	}
}

int main(void)
{
	open_sg();
	int i;
	printf("SGMAX:%d\n", SGMAX);
	int num = MAX_SIG_GROUP;//sizeof(vcb_sg_exist)/sizeof(int);
	struct timeval tvs[MAX_SIG_GROUP] = {0};
	int ts[MAX_SIG_GROUP] = {0};
	struct timeval tv;
	while(1){
		for(i = 0; i < num; i++){
			if(g_sg[i].fault == 2)
				continue;
#if 0
			if((g_sg[i].stat >= 5) && (g_sg[i].stat <=7) && (tvs[i].tv_sec == 0)){//into green stat
				//printf("=== %d ===\n", __LINE__);
				gettimeofday(&tvs[i].tv_sec, NULL);
			}
			if((g_sg[i].stat >= 1) && (g_sg[i].stat <= 4) && (tvs[i].tv_sec != 0)){//leave green stat
				//printf("=== %d ===\n", __LINE__);
				gettimeofday(&tv, NULL);
				ts[i] = (tv.tv_sec - tvs[i].tv_sec) * 10 + (tv.tv_usec - tvs[i].tv_usec) / 100000;
				tvs[i].tv_sec = 0;
			}
			printf("[%2d:%2d]", i, ts[i]);
#else
			printf("[%d]", i);
#endif
			print_signal(g_sg[i].stat, g_sg[i].time);
		}
		printf("\033[0m\r");
		us_sleep(100);
	}

	close_sg();
	return 0;
}
