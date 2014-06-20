/* 模拟产生检测器信号 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>

#include "if626max.h"
#include "tsc.h"

//int us_sleep(long us)
//{
//	struct timeval tv;
//	tv.tv_sec = 0;
//	tv.tv_usec = us;
//	return select(0, NULL, NULL, NULL, &tv);
//}

int set_rising(int index)
{
	tsc_det_op(index, 1);

	return 0;
}

int set_falling(int index)
{
	tsc_det_op(index, 2);

	return 0;
}

void set_det(void)
{
	int index, op;
	while(1){
		printf("input det num (1-45): ");
		scanf("%d", &index);
		if((index <= 0) || (index > 45)){
			printf("input num exceed\n");
			continue;
		}
		printf("select op type: 1 = set a rising slope, 2 = set a falling slope\n");
		scanf("%d", &op);
		if((op != 1) && (op != 2)){
			printf("wrong op type\n");
			continue;
		}
		tsc_det_op(index, op);
	}
}

int main(void)
{
	set_det();

	return 0;
}
