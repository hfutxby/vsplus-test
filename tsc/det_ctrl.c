/* 模拟产生检测器信号 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>

#include "if626max.h"
#include "tsc.h"
#include "vcb.h"

static char *g_det = NULL;
static int g_det_fd = 0;

/* 打开检测器模拟信息文件 */
void open_det(void)
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

void close_det(void)
{
	munmap(g_det, DETMAX);
    close(g_det_fd);
}

void set_det(void)
{
    int index, op;
    while(1){
        printf("input det num ");
        scanf("%d", &index);
        //if((index <= 0) || (index > 45)){
        //    printf("input num exceed\n");
        //    continue;
        //}
        printf("select op type: 1 = set a rising slope, 2 = set a falling slope\n");
        scanf("%d", &op);
        if((op != 1) && (op != 2)){
            printf("wrong op type\n");
            continue;
        }
        g_det[index] = op;
    }
}

void set_det_auto(int t)
{
	int index, op;
	struct timeval tv;
	int det_size = sizeof(vcb_det_exist)/sizeof(int);
	int ret;

	while(1){
		gettimeofday(&tv, NULL);
		srandom(tv.tv_usec);
		index = random() % det_size;
		if(vcb_det_exist[index] == 0)
			continue;

		gettimeofday(&tv, NULL);
		srandom(tv.tv_usec);
		op = random() % 2;
	
		printf("index:%d op:%d\n", index, op);
		g_det[index] = op + 1;
		usleep(1000 * 1000 / t);
	}
}

int main(int argc, char* argv[])
{
	open_det();

	int t = 0;
	if(argc == 2)
		t = atoi(argv[1]);
	else
		t = 1;

	if(argc == 1)
		set_det();	
	else
		set_det_auto(t);

	close_det();

	return 0;
}
