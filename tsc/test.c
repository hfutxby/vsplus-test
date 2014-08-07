#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

extern int us_sleep(long us);

int test_array(void)
{
	int inter_green[6][3] = {
		{ 1,  2,  0},
		{ 1,  3,  0},
		{ 1,  4,  0},
		{ 2, 1, 50},
		{ 2, 3,  0},
		{ 2, 4, 50}
	};

	int i, j;
	int num = sizeof(inter_green) / (sizeof(int)*3);
	printf("num:%d\n", num);
	for(i = 0; i < num; i++){
			printf("sg1:%d, sg2:%d, v:%d\n", inter_green[i][0], inter_green[i][1], inter_green[i][2]);
		}
		printf("\n");
}

int test_access(void)
{
	int ret = 0b0011;
	printf("ret:%d\n", ret);
	ret = access("/tmp/testdir", F_OK);
	if(ret != 0)
		printf("%d,%s\n", __LINE__, strerror(errno));

	ret = mkdir("/tmp/testdir", 0777);
	if(ret != 0)
		printf("%d,%s\n", __LINE__, strerror(errno));

	ret = access("/tmp/testdir", F_OK);
	if(ret != 0)
		printf("%d,%s\n", __LINE__, strerror(errno));

}

int test_mv(void)
{
	int ret;
	char buf[PATH_MAX] = {};
	printf("PATH_MAX:%d\n", PATH_MAX);
	if(getcwd(buf, sizeof(buf)) == NULL)
		printf("call getcwd error:%s\n", strerror(errno));
	printf("cwd: %s\n", buf);
	//strcat(buf, "/testlog");
	printf("target: %s\n", buf);
	ret = rename("/tmp/testlog", "testlog");
	if(ret == -1)
		printf("call rename error:%s\n", strerror(errno));
}

int test_ap(void)
{
	printf("===line:%d===\n", __LINE__);
	FILE* fp = fopen("ap.def", "rb");
	if(fp == NULL){
		perror("open file ap.det error");
		return -1;
	}
	char *line = NULL;
	int n, size;
	size = getline(&line, &n, fp);
	printf("size:%d, n:%d, line:%s\n", size, n, line);
	int a = atoi(line);
	int i = 0;
	for(i = 0; i < size; i++){
		if(line[i] == '.')
			break;
	}
	printf("i:%d\n", i);
	int b = atoi(line+i+1);
	printf("a=%d, b=%d\n", a, b);

}

#include "ring_buf.h"
int test_ring(void)
{
	ring_buf *r = alloc_ring(10);
	struct timeval tv;
	int c = 0;
	__dump(r);
	while(1){
		gettimeofday(&tv, NULL);
		srandom(tv.tv_usec);
		//c = random() % 255;
		c++;
		ring_add_over(r, 0);
		__dump(r);
		printf("sum:%d\n\n", ring_sum(r));
		usleep(1000*1000);
	}
	free_ring(r);
	return 0;
}
int main(void)
{
	test_ring();

	return 0;
}
