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
void tsc_det_fault_set(int index, int type);


//信号灯配置参数
typedef struct{
	int exist; //1：存在，0：不存在
    int min_red; //最小红灯时间
    int prep; //红绿过渡时间
    int min_green; //最小绿灯时间
    int amber; //绿红过渡时间
	int green_blink;
    int ext; //信号切换标志，1：open, 2:close
}sg_def;

//配时方案配置参数
typedef struct{
	int tu;//方案总时长
	int exist;//1：存在，0：不存在
}prg_def;

//检测器配置参数
typedef struct{
	int exist; //1：存在
}det_def;

#include "ring_buf.h"
//检测器记录信息
typedef struct {
  int sum_rising; //上升沿计数，驱动板信号修改
  int sum_falling; //下降沿计数，驱动板信号修改
  int state; //占用状态，驱动板信号修改
  int hold; //总占用计时
  int free; //总空闲计时
  int fault; //1:故障,0:正常
  double occ1;//占用率
  double occ2;//平滑占用率
  int net;//time gap starts at the last falling slope，驱动>板信号修改
  int gross;
  ring_buf* rh;
}det_track;

typedef struct{
	int det_sg[DETMAX];//检测器和信号灯对应关系
	int sg[SGMAX];
}xml_para;

#if 1
typedef struct{
	short head;
	short MP;// (call point);
	short Linie;// (line);
	short Kurs;// (course);
	short Route;// (route);
	short Prioritaet;// (priority);
	short Laenge;// (vehicle length);
	short RichtungVonHand;// (direction by hand);
	short FahrplanAbweichnung;// (difference to schedule);
}PTMSG;
#else
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
#endif
//xml_para* g_xml_para = NULL;

#endif /*__TSC_H*/
