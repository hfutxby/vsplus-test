#include <stdio.h>
#include <stdlib.h>
#include "if626bas.h"
#include "if626max.h"
#include "vsp626ste.h"
#include "VSP_Interface.h"
#include "tsc.h"

pthread_t g_tid_vsplus;
static int g_exit = 0;
volatile int g_vsplus_ret = -1;

typedef struct{
	VSPSollTyp vsp_soll[GERAET_TEILKNOTEN_MAX];// = {VSP_ND};
	WBTyp sg_mode[SGMAX];// = {0};
	WBReadyTyp wb_ready[GERAET_TEILKNOTEN_MAX];// = {0};
}VS_PARA;

int thr_vsplus(void* arg)
{
	VS_PARA* vs_para = (VS_PARA*)arg;
	while(!g_exit){
		if(g_vsplus_ret == -1){
			vs_para->vsp_soll[0] = VSP_EIN;
			g_vsplus_ret = VSPLUS(vs_para->vsp_soll, vs_para->sg_mode, vs_para->wb_ready);
		}
	}
	
}

int main(void)
{
	int ret = 0;
	int i;
	tsc_init(); //控制器初始化
	//sleep(2);
#if 1
	//初始化参数存储区
	ret = Initial_VSP_Parameter();
	printf("%s(%d):Initial_VSP_Parameter ret=%d\n", __func__, __LINE__, ret);
	if(ret < 0)
		return -1;
#endif
#if 10
	//检查参数配置文件（vcb文件）
	ret = Pruefen_VSP_Parameter();
	printf("%s(%d):Pruefen_VSP_Parameter ret=%d\n", __func__, __LINE__, ret);
	if(ret < 0)
		return -1;
#endif
#if 10
	//读参数配置文件（vcb文件）
	ret = Lesen_VSP_Parameter();
	printf("%s(%d):Lesen_VSP_Parameter ret=%d\n", __func__, __LINE__, ret);
	if(ret < 0)
		return -1;
#endif
#if 10
	//初始化VSPLUS
	//VSPLUS(VSPSollTyp* VSPSoll, WBTyp* SchaltBild, WBReadyTyp* WunschBildBereit)
//	VSPSollTyp vsp_soll[GERAET_TEILKNOTEN_MAX] = {VSP_ND};
//	WBTyp sg_mode[SGMAX] = {0};
//	WBReadyTyp wb_ready[GERAET_TEILKNOTEN_MAX] = {0};
//	vsp_soll[0] = VSP_NEU_INI;
//	wb_ready[0] = 0;
	VS_PARA vs_para;
	for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		vs_para.vsp_soll[i] = VSP_ND;
	for(i = 0; i <SGMAX; i++)
		vs_para.sg_mode[i] = 0;
	for(i = 0; i <GERAET_TEILKNOTEN_MAX; i++)
		vs_para.wb_ready[i] = 0;

	vs_para.vsp_soll[0] = VSP_NEU_INI;
	vs_para.wb_ready[0] = 0;
	if((ret = VSPLUS(vs_para.vsp_soll, vs_para.sg_mode, vs_para.wb_ready)) >= 0){
		printf("call VSPLUS success, ret=%d\n", ret);
	}
	else{
		printf("call VSPLUS fail, ret=%d\n", ret);
	}
#endif
#if 1
	for(i = 1; i < 256; i++){
		ret = l_Prog_VSP(i);
		if(ret != 0)
			printf("%d:%d ", i, ret);
	}
	printf("\n");
#endif
#if 0
	//fix time run
	while(1){
		vsp_soll[0] = VSP_AUS;
		ret = VSPLUS(vsp_soll, sg_mode, wb_ready);
		sleep(1);
	}
#endif
#if 10
	//full run
	vs_para.vsp_soll[0] = VSP_NEU;
	ret = VSPLUS(vs_para.vsp_soll, vs_para.sg_mode, vs_para.wb_ready);
	pthread_create(&g_tid_vsplus, NULL, thr_vsplus, &vs_para);
	while(!g_exit){
		g_vsplus_ret = -1;
		us_sleep(1000000);//1s
		if(g_vsplus_ret == -1){
			printf("===>need switch to fix run\n");
			vs_para.vsp_soll[0] = VSP_AUS;
		}
		else{
			vs_para.vsp_soll[0] = VSP_EIN;
		}
		printf("==============%d\n", vs_para.vsp_soll[0]);// = VSP_EIN;
	}

#endif
	Ende_VSP_Parameter();
	tsc_deinit();

	return 0;
}

