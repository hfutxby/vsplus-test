/* 模拟产生检测器信号 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>

#include "if626max.h"
#include "tsc.h"

static char *g_det = NULL;
static int g_det_fd = 0;
static int g_exit_det_read = 0;
static pthread_t g_tid_det_read;

/* 打开检测器模拟信息文件 */
void open_det_ctrl(void)
{
    int ret;
    g_det_fd = open("det_ctrl.dat", O_RDWR|O_CREAT, 0644);
    if(g_det_fd < 0){
        printf("%s\n", strerror(errno));
        return;
    }
    if(ret = ftruncate(g_det_fd, DETMAX) < 0){
        printf("%s\n", strerror(errno));
    }
    g_det = mmap(NULL, DETMAX,  PROT_READ|PROT_WRITE, MAP_SHARED, g_det_fd, 0);
    if(g_det ==  MAP_FAILED){
        printf("%s\n", strerror(errno));
    }
    memset(g_det, 0, DETMAX);//1:rising, 2:falling
}

void close_det_ctrl(void)
{
	munmap(g_det, DETMAX);
    close(g_det_fd);
}

void* thr_det_read(void)
{
	int i, ret;
	while(!g_exit_det_read){
		for(i = 0; i < DETMAX; i++){
			if(g_det[i] == 1){
				printf("rising slope:%d\n", i);
				tsc_det_op(i, 1);
				g_det[i] = 0;
			}
			else if(g_det[i] == 2){
				printf("falling slope:%d\n", i);
				tsc_det_op(i, 2);
				g_det[i] = 0;
			}
			else
				;
		}
		usleep(1000);
	}
}
#if 0
int main(void)
{
	open_det_ctrl();

	int i, ret;
	while(1){
		for(i = 0; i < DETMAX; i++){
			if(g_det[i] == 1){
				printf("rising slope:%d\n", i);
				g_det[i] = 0;
			}
			else if(g_det[i] == 2){
				printf("falling slope:%d\n", i);
				g_det[i] = 0;
			}
			else
				;
		}
	}

	close_det_ctrl();

	return 0;
}

#else
int det_read_init(void)
{
	open_det_ctrl();

	pthread_create(&g_tid_det_read, NULL, thr_det_read, NULL);	

	return 0;
}

void det_read_deinit(void)
{
	if(g_tid_det_read)
		pthread_join(g_tid_det_read);

	close_det_ctrl();
}
#endif
