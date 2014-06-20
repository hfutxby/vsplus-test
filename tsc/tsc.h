#ifndef __TSC_H
#define __TSC_H

#include "if626max.h"

/* 出错信息 */
#if(DEBUG>=1)
#define debug1(fmt, args...) \
printf("\033[40;31m(%s:%d)\033[0m"fmt, __func__, __LINE__, ##args) 
#else
#define debug1(fmt, args...) 
#endif /* DEBUG>=1 */

/* 警告信息 */
#if(DEBUG>=2)
#define debug2(fmt, args...) \
printf("\033[40;32m(%s:%d)\033[0m"fmt, __func__, __LINE__, ##args) 
#else
#define debug2(fmt, args...) 
#endif /* DEBUG>=2 */

/* 调试信息 */
#if(DEBUG>=3)
#define debug3(fmt, args...) \
printf("\033[40;33m(%s:%d)\033[0m"fmt, __func__, __LINE__, ##args) 
#else
#define debug3(fmt, args...) 
#endif /* DEBUG>=3 */

#if 1
#define debug(level, fmt, args...) debug##level(fmt, ##args)
#else
#define debug(level, fmt, args...) printf("(%s:%d)"fmt, __func__, __LINE__, ##args)
#endif

void* tsc_alloc_mem(int size, int id);
void tsc_free_mem(int id);
void* tsc_get_mem(int id);
int tsc_open_vcb(void);
void tsc_close_vcb(void);
int tsc_read_vcb(char *data, int size);
int tsc_prog_actual(void);
int tsc_prog_select(void);
int tsc_prog_tx(void);
int tsc_prog_tu(void);
void tsc_stream_waiting(int index, int time);

#define MAXCOUNT (32767<<1)

/****** 控制器程序调用接口 ******/
void tsc_det_op(int index, int op);


////信号灯记录信息
//typedef struct{
//    int stat; //信号灯状态，0-disable;1-amber;2-min_red;3-ex_red;4-prep;5-min_green;6-ex_green
//    int red_min; //最小红灯时间设定
//    int prep; //红绿过渡时间设定 
//    int green_min; //最小绿灯时间设定
//    int amber;//绿红过渡时间设定
//    int time; //信号灯状态计时
//    int ext;//b00状态未变，b01进行红绿切换，b10进行绿红切换
//}sg_node;

//信号灯配置参数
typedef struct{
	int fault; //1：不存在，2：存在但是故障了，0：正常
    int red_min; //最小红灯时间
    int prep; //红绿过渡时间
    int green_min; //最小绿灯时间
    int amber; //绿红过渡时间
    int ext; //信号切换标志，1：open, 2:close
}sg_def;

//检测器记录信息
typedef struct {
    int sum_rising; //上升沿计数
    int sum_falling; //下降沿计数
    int state; //计时状态，<100:占用中，>100:空闲中
    int hold; //总占用计时
    int free; //总空闲计时
    int fault; //故障
    int occ1;//占用率
    int occ2;//平滑占用率
    int net;//net time gap starts at the last falling slope
    int gross;//gross time gap starts at the last rising slope
}det_node;

typedef struct{
	int det_sg[DETMAX];//检测器和信号灯对应关系
	int sg[SGMAX];
}xml_para;

typedef struct{
	int MP;// (call point);
	int Linie;// (line);
	int Kurs;// (course);
	int Route;// (route);
	int Prioritaet;// (priority);
	int Laenge;// (vehicle length);
	int RichtungVonHand;// (direction by hand);
	int FahrplanAbweichnung;// (difference to schedule);
}PTMSG;

//xml_para* g_xml_para = NULL;

#endif /*__TSC_H*/
