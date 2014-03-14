/* vsplus模拟运行需要补充的定义
 * 头文件不完整造成
 * 公共定义
 */

#ifndef __SIM_VSPLUS_H
#define __SIM_VSPLUS_H

#include "if626bas.h"

#define SGMAX 5
#define DETMAX 340
#define VSMAX 64
#define SPEZMAX 16
#define MAXTIMER 1912

//#define DEBUG
#define d_printf(fmt, args...)  printf("\033[40;33m(%s:%d)\033[0m"fmt, __func__, __LINE__, ##args);
//level=1,track; level=2,watch;
#if DEBUG
#define debug(level, fmt, args...) \
if(DEBUG==level) \
printf("\033[40;33m(%s:%d)\033[0m"fmt, __func__, __LINE__, ##args);
#else
#define debug(level, fmt, args...)
#endif

typedef unsigned char WBTyp;
typedef unsigned char WBReadyTyp;

typedef enum {
	VSP_AUS = 0,
	VSP_EIN = 1,
	VSP_NEU = 2,
	VSP_WUNSCH_AUS_UM = 3,
	VSP_NEU_INI = 4,
	VSP_ND = 99
} VSPSollTyp;

#endif /*__SIM_VSPLUS_H*/
