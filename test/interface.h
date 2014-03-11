/*
 * 为VSPLUS提供运行接口
 */

#ifndef __INTERFACE_H_
#define __INTERFACE_H_

int timer(int func, int index, int count);
short timer_2(short funktion, short timer, short wert);
short ProgrammAktuell(void);
short ProgrammWahl(void);

/* 附加定义，应该已经在缺失的头文件中有过定义 */
#if 1
typedef enum {
	VSP_AUS = 0,
	VSP_EIN = 1,
	VSP_NEU = 2,
	VSP_WUNSCH_AUS_UM = 3,
	VSP_NEU_INI = 4,
	VSP_ND = 99
} VSPSollTyp;

#endif

#endif /*__INTERFACE_H_*/
