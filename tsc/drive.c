//驱动板功能
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#include "drive.h"
#include "tsc.h"
#include "serial_pack.h"
#include "parse_xml.h"
#include "vcb.h"

/*====== 信号灯操作 =======*/
//取得信号灯编号的代码
int drv_sg_code(int sg)
{
#if 0
	int xml_sg_code(int sg);
#else
	if(sg < (sizeof(vcb_sg_code) / sizeof(int)))
		return vcb_sg_code[sg];
	else
		return 0;
#endif
}

//取得信号灯状态的代码
int drv_sg_stat_code(int stat)
{
	if(stat < (sizeof(vcb_sg_stat_code) / sizeof(int)))
		return vcb_sg_stat_code[stat];
	else
		return -1;
}

//切换信号灯并记录状态
void drv_sg_switch(int sg, int stat)
{
	//printf("==========drv_sg_switch(%d, %d)===========\n", sg, stat);
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
	memset(ptr, 0, size);
	int num = size / sizeof(sg_def);
	int num_a = sizeof(vcb_sg_exist) / sizeof(int);
	num = (num > num_a) ? num_a : num;
	sg_def* sg = NULL;
	for(i = 0; i < num; i++){
		sg = (sg_def*)(ptr + i * sizeof(sg_def));
		sg->min_red = vcb_close_sum[i];
		sg->min_green = vcb_free_sum[i] - vcb_prep[i];//green_blink作为close
		sg->prep = vcb_prep[i];
		sg->amber = vcb_amber[i];
		sg->green_blink = vcb_green_blink[i];
		sg->ext = 0;
		sg->exist = vcb_sg_exist[i];
	}

	return 0;
#endif
}

void drv_sg_para_dump(void* ptr, int size)
{
	int i;
	int num = size / sizeof(sg_def);
	sg_def* sg = NULL;
	printf("===%s===\n", __func__);
	//printf("sg_id\t min_red\t min_green\t prep\n");
	printf("%9s %9s %9s %9s\n", "sg_id", "min_red", "min_green", "prep");
	for(i = 0; i < num; i++){
		sg = (sg_def*)(ptr + i * sizeof(sg_def));
		if(sg->exist)
			printf("%9d %9d %9d %9d\n", i, sg->min_red, sg->min_green, sg->prep);
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
	memset(ptr, 0, size);
	int i;
	det_def* det = NULL;
	int num = size / sizeof(det_def);
	int num_a = sizeof(vcb_det_exist) / sizeof(int);
	num = (num > num_a) ? num_a : num;
	debug(3, "num:%d\n", num);
	for(i = 0; i < num; i++){
		det = (det_def*)(ptr + i * sizeof(det_def));
		det->exist = vcb_det_exist[i];
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
//prg_def参数
int drv_prg_para(void* ptr, int size)
{
#if 0
	int xml_prg_para(void* ptr, int size);
#else
	memset(ptr, 0, size);
	int i;
	prg_def* prg = NULL;
	int num = size / sizeof(prg_def);
	int num_a = sizeof(vcb_prg_exist) / sizeof(int);
	num = (num > num_a) ? num_a : num;
	debug(3, "ptr:%p, size:%d, num:%d\n", ptr, size, num);
	for(i = 0; i < num; i++){
		prg = (prg_def*)(ptr + i * sizeof(prg_def));
		prg->exist = vcb_prg_exist[i];
		prg->tu = vcb_prg_tu[i];
	}
	return 0;
#endif
}

void drv_prg_para_dump(void* ptr, int size)
{
	int num = size / sizeof(prg_def);
	int i;
	prg_def* prg = NULL;
	printf("===%s===\n", __func__);
	printf("%6s %6s\n", "prg_id", "tu");
	for(i = 0; i < num; i++){
		prg = (prg_def*)(ptr + i * sizeof(prg_def));
		if(prg->exist)
			printf("%6d %6d\n", i, prg->tu);
	}
}

int drv_prg_next_set(int index)
{
	return prg_track_next_set(index);
}

int drv_inter_green(int sgr, int sge)
{
	int num = sizeof(vcb_inter_green) / (sizeof(int)*3);
	int i;
	for(i = 0; i < num; i++){
		if((vcb_inter_green[i][0] == sgr) && (vcb_inter_green[i][1] == sge)){
			return vcb_inter_green[i][2];
		}
	}

	return 0;
	//return 32000;
}

int drv_get_ocitid(int* ZNr, int* FNr, int*Realknoten)
{
	*ZNr = vcb_ZNr;
	*FNr = vcb_FNr;
	*Realknoten = vcb_Realknoten;

	return 1;
}

static struct timeval g_first_tv = {};
//write ini file
int drv_add_det(int det, int value)
{
	//time_t tt = time(NULL);
	struct timeval tv;
	gettimeofday(&tv, NULL);
	time_t tt = tv.tv_sec;
    struct tm *t = localtime(&tt);
	char str[256] = {};
	sprintf(str, "log/det/%d_%04d%02d%02d%02d%02d%02d.ini", vcb_FNr, t->tm_year+1900, 
		t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	if(access(str, F_OK) == -1){
		debug(2, "first create file %s\n", str);
		g_first_tv = tv;
	}
	FILE* fp = fopen(str, "a+");
	if(fp == NULL){
		debug(1, "open file %s error\n", str);
	}
	memset(str, 0, sizeof(str));
	int diff = (tv.tv_usec/10000 - g_first_tv.tv_usec/10000);
	fprintf(fp, "[%d_D_%d %d]\n", vcb_FNr, det, diff*10);
	fprintf(fp, "Value=%d\n", value);
	fprintf(fp, "Time=%04d%02d%02d%02d%02d%02d%03d\n\n", t->tm_year+1900, 
        t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, (tv.tv_usec/10000)*10);

	fclose(fp);
}
