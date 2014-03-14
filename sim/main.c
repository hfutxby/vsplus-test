/* 模拟测试vsplus */

#include <stdio.h>
#include "tsc.h"
#include "sim_signal.h"
#include "sim_vsplus.h"
#include "interface.h"

int test_timer(void)
{
	init_timers();
	int func, index, count, ret;
	while(1){
		pr_info();
		scanf("%d %d %d", &func, &index, &count);
		if(func == 1){
			ret = _timer(func, index, count);
			printf("timer[%d],count=%d\n", index, ret);
		}
		else{
			ret = _timer(func, index, count);
		}
	}
}

int test_program(void)
{
	init_signal();
	int i = 0;
	while(1){
		i++;
		sleep(1);
		printf("program_actual:%d\n", program_actual());
		if(i%3 == 0)
			printf("\033[40;33mprogram_selected:%d\033[0m\n", program_selected());
	}

	return 0;
}
#if 0
int test_vsplus(void)
{
	init_timers();
	init_signal();

//	VSPLUS(VSPSollTyp* VSPSoll, WBTyp* SchaltBild, WBReadyTyp* WunschBildBereit)
	VSPSollTyp VSPSoll[GERAET_TEILKNOTEN_MAX];
	WBTyp SchaltBild[SGMAX];
	WBReadyTyp WunschBildBereit[GERAET_TEILKNOTEN_MAX];
}
#endif

int test_vsplus(void)
{
	init_tsc();//初始化控制器
	init_signal();
	int ret = 0;
	ret = Initial_VSP_Parameter();//初始化参数存储区
#if 0
	VSPSollTyp VSPSoll[GERAET_TEILKNOTEN_MAX] = {VSP_ND};
	WBTyp SchaltBild[SGMAX];
	WBReadyTyp WunschBildBereit[GERAET_TEILKNOTEN_MAX];

	int i;
	for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++){
		printf("VSPSoll[%d]=%d\n", i, VSPSoll[i]);
	}
#endif
#if 0
	VSPLUS(VSP_NEU_INI...);//初始化vsplus
	VSPLUS(VSP_AUS...);//关闭vsplus功能，等待参数设置
	PRUEFEN_VSP_PARAMETER;//检查参数有效性
	LESEN_VSP_PARAMETER();//设置参数

	VSPLUS(VSP_NEU...);
	while(sleep(1)){
		VSPLUS(VSP_EIN...);
	}
#endif

	return 0;
}

int main(void)
{
    test_vsplus();
    //printf("GERAET_TEILKNOTEN_MAX=%d\n", GERAET_TEILKNOTEN_MAX);
    //printf("DETMAX=%d\n", DETMAX);
    
    return 0;
}
