//驱动板功能
#include <stdio.h>

#include "drive.h"
#include "tsc.h"
#include "parse_xml.h"

/******* 信号灯操作 **********/
//取得信号灯编号的代码
int drv_sg_code(int sg)
{
#if 0
	xml_sg_code(int* sg, int size);
#else
	int sg_code[] = {0,//匹配信号灯测试面板
		0x41,//D1
		0x11,//D2
		0x22,//D3
		0x43,//D4
		0x23,//D5
		0x24,//D6
		0x62,0x63,0x64,0x61,//F7-F10
		0,0,//FL11-FL12
		//0,0,0,0,0,0,//DR1-6
		0,//D13
		0,//D14
		0};//F15

	if(sg < (sizeof(sg_code) / sizeof(int)))
		return sg_code[sg];
	else
		return 0;
#endif
}

//取得信号灯状态的代码
int drv_sg_stat_code(int stat)
{
	int sg_stat_code[] = {
		0x00, //dark
		0x02, //amber
		0x01, //min-red
		0x01, //ext-red
		0x02, //prep
		0x04, //min-green
		0x04, //ext-green
		0x11, //红闪
		0x12, //黄闪
		0x14 //绿闪
	};
	if(stat < (sizeof(sg_stat_code) / sizeof(int)))
		return sg_stat_code(sg);
	else
		return -1;
}

//切换信号灯并记录状态
void drv_sg_switch(int sg, int stat)
{
	int
	unsigned char msg[4];
	msg[0] = 0x96; msg[3] = 0x69;
	msg[1] = drv_sg_code(sg);
	msg[2] = drv_sg_stat_code(stat);
	
	serial_command(msg, 4);//通过串口切换信号灯
	sg_track_switch(sg, stat);//记录信号灯状态信息	
}

//检查信号灯的状态
int drv_sg_chk(int sg, int stat)
{
	int ret;
	ret = sg_track_chk(sg, stat);

	return ret;
}

//获取信号灯配置
//
int drv_sg_para(void* ptr)
{
#if 0
	xml_sg_para(ptr);
#else
	int i;
	int red_min[16] = {40,40,40,40,40,40,40,80,80,80,80,40,40,40,40,60};
	int green_min[16] = {20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};
	sg_def* sg = NULL;	
	for(i = 0; i < 16; i++){
		sg = (sg_def*)(ptr + i * sizeof(sg_def));
		sg->red_min = red_min[i];
		sg->green_min = green_min[i];
		sg->prep = 30;
		sg->amber = 30;
		sg->ext = 0;
		sg->fault = 0;
	}
	for(i = 16; i < SGMAX; i++){
		sg = (sg_def*)(ptr + i * sizeof(sg_def));
		memset(sg, 0, sizeof(sg_def));
		sg->fault = 1;
	}
#endif
}

/******* 串口操作 ********/

/********* vsplus提供的调用函数 ***********/
void drv_det_op(int index, int op)
{
	tsc_det_op(index, op);
}
