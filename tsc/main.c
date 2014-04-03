#include <stdio.h>
#include <stdlib.h>
#include "if626bas.h"
#include "if626max.h"
#include "vsp626ste.h"
#include "VSP_Interface.h"
#include "tsc.h"

int main(void)
{
	int ret = 0;
	tsc_init(); //控制器初始化
	//sleep(2);
#if 1
	//初始化参数存储区
	ret = Initial_VSP_Parameter();
	printf("%s(%d):Initial_VSP_Parameter ret=%d\n", __func__, __LINE__, ret);
#endif
#if 10
	//检查参数配置文件（vcb文件）
	ret = Pruefen_VSP_Parameter();
	printf("%s(%d):Pruefen_VSP_Parameter ret=%d\n", __func__, __LINE__, ret);
#endif
#if 10
	//读参数配置文件（vcb文件）
	ret = Lesen_VSP_Parameter();
	printf("%s(%d):Lesen_VSP_Parameter ret=%d\n", __func__, __LINE__, ret);
#endif
//int i;
//for(i = 1; i < 256; i++){
//	ret = l_Prog_VSP(i);
//	printf("%d:%d ", i, ret);
//}
#if 10
	//初始化VSPLUS
	//VSPLUS(VSPSollTyp* VSPSoll, WBTyp* SchaltBild, WBReadyTyp* WunschBildBereit)
	VSPSollTyp vsp_soll[GERAET_TEILKNOTEN_MAX] = {VSP_ND};
	WBTyp sg_mode[SGMAX] = {0};
	WBReadyTyp wb_ready[GERAET_TEILKNOTEN_MAX] = {0};
	vsp_soll[0] = VSP_NEU_INI;
	wb_ready[0] = 0;
	if((ret = VSPLUS(vsp_soll, sg_mode, wb_ready)) >= 0){
		printf("call VSPLUS success, ret=%d\n", ret);
	}
	else{
		printf("call VSPLUS fail, ret=%d\n", ret);
	}
#endif

#if 0
	//fix time run
	while(1){
		vsp_soll[0] = VSP_AUS;
		ret = VSPLUS(vsp_soll, sg_mode, wb_ready);
		sleep(1);
	}
#endif
#if 0
	//full run
	vsp_soll[0] = VSP_NEU;
	ret = VSPLUS(vsp_soll, sg_mode, wb_ready);
	while(1){
		vsp_soll[0] = VSP_EIN;
		if((ret = VSPLUS(vsp_soll, sg_mode, wb_ready))>=0){
			printf("call VSPLUS success, ret=%d\n", ret);
		}
		else{
			printf("call VSPLUS fail, ret=%d\n", ret);
		}

		sleep(1);
	}
#endif
	Ende_VSP_Parameter();
	tsc_deinit();

	return 0;
}

