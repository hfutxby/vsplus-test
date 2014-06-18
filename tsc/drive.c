//驱动板功能
#include <stdio.h>

#include "drive.h"


/******* 检测器 **********/

/* 维护检测器数据，频率100ms */
void* thr_det(void* arg)
{

}

/* 更新检测器数据 */
int det_op(int op)
{

}

/******* 串口操作 ********/
void* thr_serial(void* arg)
{
	det_op();

}
