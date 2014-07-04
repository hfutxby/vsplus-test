//驱动板功能
#include <stdio.h>
#include <string.h>

#include "drive.h"
#include "tsc.h"
#include "serial_pack.h"
#include "parse_xml.h"

/*====== 信号灯操作 =======*/
//取得信号灯编号的代码
int drv_sg_code(int sg)
{
#if 0
	int xml_sg_code(int sg);
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
		return sg_stat_code[stat];
	else
		return -1;
}

//切换信号灯并记录状态
void drv_sg_switch(int sg, int stat)
{
	printf("==========drv_sg_switch(%d, %d)===========\n", sg, stat);
	unsigned char msg[4];
	msg[0] = 0x96; msg[3] = 0x69;
	msg[1] = drv_sg_code(sg);
	msg[2] = drv_sg_stat_code(stat);
	
	serial_command(msg, 4);//通过串口切换信号灯
	sg_track_switch(sg, stat);//记录信号灯状态信息	
}

#if 0
//检查信号灯的状态
//如果sg处于stat状态，返回此状态时间，否则返回-1
int drv_sg_chk(int sg, int stat)
{
	int ret;
	ret = sg_track_chk(sg, stat);

	return ret;
}
#endif

//获取信号灯配置
//ptr为sg_def结构
int drv_sg_para(void* ptr, int size)
{
#if 0
	int xml_sg_para(void* ptr, int size);
#else
	int i;
	int min_red[16] = {0,40,40,40,40,40,40,80,80,80,80,40,40,40,40,60};
	int min_green[16] = {0,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};
	sg_def* sg = NULL;	
	int num = size / sizeof(sg_def);
	//for(i = 0; i < num; i++){
	//	sg = (sg_def*)(ptr + i * sizeof(sg_def));
	//	memset(sg, 0, sizeof(sg_def));
	//	sg->fault = 1;//sg不存在
	//}
	num = (num > 16) ? 16 : num;
	for(i = 1; i <= num; i++){
		sg = (sg_def*)(ptr + i * sizeof(sg_def));
		sg->min_red = min_red[i];
		sg->min_green = min_green[i];
		sg->prep = 30;
		sg->amber = 30;
		sg->ext = 0;
		sg->exist = 1;
	}
	
	return 0;
#endif
}

void drv_sg_para_dump(void* ptr)
{
	if(!ptr)
		return;
	
	int i;
	sg_def* sg = NULL;
	for(i = 0; i < 16; i++){
		sg = (sg_def*)(ptr + i * sizeof(sg_def));
		if(sg->exist)
			printf("%2d: %3d %3d %3d\n", i, sg->min_red, sg->min_green, sg->prep);
	}
}

#if 0
//检查信号灯是否故障
int drv_sg_fault(int index)
{
	return sg_track_fault(index);
}
#endif

/*===== 检测器操作 ======*/
#if 0
void drv_det_op(int index, int op)
{
	tsc_det_op(index, op);
}
#endif

//获取检测器配置参数
int drv_det_para(void* ptr, int size)
{
#if 0
	int xml_det_para(void* ptr, int size);
#else
	int num = size / sizeof(det_def);
	memset(ptr, 0, size);
	int i;
	det_def* det = NULL;
	num = (num > 45) ? 45 : num;
	debug(3, "num:%d\n", num);
	for(i = 1; i <= num; i++){
		det = (det_def*)(ptr + i * sizeof(det_def));
		det->exist = 1;
	}
	return 0;
#endif
}

/*=== 串口命令 ===*/
//处理串口命令
int drv_handle_pack(unsigned char* buf)
{
	printf("command: 0x%x\n", buf[0]);

	return 0;
}

/*===== 配时方案 ====*/
int drv_prg_para(void* ptr, int size)
{
#if 0
	int xml_prg_para(void* ptr, int size);
#else
	int num = size / sizeof(prg_def);
	debug(3, "num:%d\n", num);//FIXME
	void* p = memset(ptr, 0, size);
	debug(3, "p=%p, ptr=%p\n", p, ptr);
	int i;
	prg_def* prg = NULL;
	num = (num > 7) ? 7 : num;
	debug(3, "ptr:%p, size:%d\n", ptr, size);//FIXME
	debug(3, "num:%d\n", num);//FIXME
	for(i = 1; i < num; i++){
		prg = (prg_def*)(ptr + i * sizeof(prg_def));
		prg->exist = 1;
		prg->tu = 720;//72s
	}
	return 0;
#endif
}

void drv_prg_para_dump(void* ptr, int size)
{
	int num = size / sizeof(prg_def);
	int i;
	prg_def* prg = NULL;
	num = (num > 7) ? 7 : num;
	for(i = 0; i < num; i++){
		prg = (prg_def*)(ptr + i * sizeof(prg_def));
		printf("%d:%d %d\n", i, prg->exist, prg->tu);
	}
}

int drv_prg_next_set(int index)
{
	return prg_track_next_set(index);
}
