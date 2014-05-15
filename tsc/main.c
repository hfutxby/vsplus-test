#include <stdio.h>
#include <stdlib.h>
#include "if626bas.h"
#include "if626max.h"
#include "vsp626ste.h"
#include "VSP_Interface.h"
#include "tsc.h"

static int g_exit = 0;
int g_sleep = 0;//测试用，延长vsplus运行时间，使进入fix mode
pthread_t g_tid_sleep;
int g_fix_mode = 0;//1=初次进入fix，2=已进入fix，3=初次离开fix进入vsplus，4=已进入vsplus
pthread_t g_tid_fix;
volatile int g_vsplus_ret = -1;
pthread_t g_tid_vsplus;

typedef struct{
	VSPSollTyp vsp_soll[GERAET_TEILKNOTEN_MAX];// = {VSP_ND};
	WBTyp sg_mode[SGMAX];// = {0};
	WBReadyTyp wb_ready[GERAET_TEILKNOTEN_MAX];// = {0};
}VS_PARA;
VS_PARA g_vs_para;

int thr_sleep(void* arg)
{
	int tmp;
	while(!g_exit){
		scanf("%d", &tmp);
		if((tmp > 0) && (tmp < 100)){
			g_sleep = tmp;
		}
		else
			continue;
	}

	return 0;
}

int thr_fix_run(void* arg)
{
	int count = 0;
	while(!g_exit){
		if(g_fix_mode == 1){
			printf("first into fix mode\n");
			g_fix_mode = 2;
			count = 5;
		}
		if(g_fix_mode == 2){
			printf("waiting return to vsplus, %d\n", count);
			count--;
			if(count == 0)
				g_fix_mode = 3;
		}
		sleep(1);
	}

	return 0;
}

int thr_vsplus(void* arg)
{
	//VS_PARA* vs_para = (VS_PARA*)arg;
	while(!g_exit){
		if(g_vsplus_ret == -1){
			printf("===VSPLUS Call===\n");
			us_sleep(100000*g_sleep);
			if(g_fix_mode == 3){//初次进入vsplus
				g_vs_para.vsp_soll[0] = VSP_NEU;
				g_vsplus_ret = VSPLUS(g_vs_para.vsp_soll, g_vs_para.sg_mode, g_vs_para.wb_ready);
				g_fix_mode = 4;
			}
			if(g_fix_mode == 4){//已进入vsplus
				g_vs_para.vsp_soll[0] = VSP_EIN;
				g_vsplus_ret = VSPLUS(g_vs_para.vsp_soll, g_vs_para.sg_mode, g_vs_para.wb_ready);
			}
			else{//VSP_AUS
				VSPLUS(g_vs_para.vsp_soll, g_vs_para.sg_mode, g_vs_para.wb_ready);
			}
		}
	}
	
}

int main(void)
{
	int ret = 0;
	int i;
	tsc_init(); //控制器初始化
	//sleep(2);
#if 0
	//初始化参数存储区
	ret = Initial_VSP_Parameter();
	printf("%s(%d):Initial_VSP_Parameter ret=%d\n", __func__, __LINE__, ret);
	if(ret < 0)
		return -1;
#endif
#if 1
	//初始化参数存储区
	ret = vs_init_parameter();
	printf("%s(%d):vs_init_parameter ret=%d\n", __func__, __LINE__, ret);
	if(ret < 0)
		return -1;
#endif
#if 0
	//检查参数配置文件（vcb文件）
	ret = Pruefen_VSP_Parameter();
	printf("%s(%d):Pruefen_VSP_Parameter ret=%d\n", __func__, __LINE__, ret);
	if(ret < 0)
		return -1;
#endif
#if 1
	//检查参数配置文件（vcb文件）
	ret = vs_chk_parameter();
	printf("%s(%d):vs_chk_parameter ret=%d\n", __func__, __LINE__, ret);
	if(ret < 0)
		return -1;
#endif
#if 0
	//读参数配置文件（vcb文件）
	ret = Lesen_VSP_Parameter();
	printf("%s(%d):Lesen_VSP_Parameter ret=%d\n", __func__, __LINE__, ret);
	if(ret < 0)
		return -1;
#endif
#if 10
	//读参数配置文件（vcb文件）
	ret = vs_read_parameter();
	printf("%s(%d):vs_read_parameter ret=%d\n", __func__, __LINE__, ret);
	if(ret < 0)
		return -1;
#endif
#if 1
	char text[128];
	ret = vs_version(text, sizeof(text));
	if(ret)
		printf("vsplus version: %s\n", text);
	else
		printf("vsplus version info not available\n");
#endif
#if 10
	//初始化VSPLUS
	printf("GERAET_TEILKNOTEN_MAX:%d, SGMAX:%d\n", GERAET_TEILKNOTEN_MAX, SGMAX);
	for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.vsp_soll[i] = 0;//VSP_ND;
	for(i = 0; i < SGMAX; i++)
		g_vs_para.sg_mode[i] = 0;
	for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.wb_ready[i] = 0;

	g_vs_para.vsp_soll[0] = VSP_NEU_INI;
	g_vs_para.wb_ready[0] = 0;
	if((ret = VSPLUS(g_vs_para.vsp_soll, g_vs_para.sg_mode, g_vs_para.wb_ready)) >= 0){
		printf("call VSPLUS success, ret=%d\n", ret);
	}
	else{
		printf("call VSPLUS fail, ret=%d\n", ret);
	}
#endif
#if 1
	//检查program是否存在
	for(i = 1; i < 256; i++){
		ret = vs_prog_chk(i);
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
	g_vs_para.vsp_soll[0] = VSP_NEU;
	if((ret = VSPLUS(g_vs_para.vsp_soll, g_vs_para.sg_mode, g_vs_para.wb_ready)) >= 0){
		printf("call VSPLUS success, ret=%d\n", ret);
		g_fix_mode = 4;
	}
	else{
		printf("call VSPLUS fail, ret=%d\n", ret);
	}

	pthread_create(&g_tid_sleep, NULL, thr_sleep, NULL);
	pthread_create(&g_tid_fix, NULL, thr_fix_run, NULL);

	pthread_create(&g_tid_vsplus, NULL, thr_vsplus, NULL);

	while(!g_exit){//如果VSPLUS函数未能在1s内返回则关闭VSPLUS
		if(g_fix_mode == 4){
			g_vsplus_ret = -1;
			us_sleep(1000000);//1s
			if(g_vsplus_ret == -1){
				printf("===>need switch to fix run\n");
				g_vs_para.vsp_soll[0] = VSP_AUS;
				g_fix_mode = 1;//1=初次进入fix，2=已进入fix，3=初次离开fix进入vsplus，4=已进入vsplus
			}
			else{
				g_vs_para.vsp_soll[0] = VSP_EIN;
				g_fix_mode = 4;
			}
		}
		else{
			us_sleep(1000000);//1s
		}
		printf("vsplus run mode:%d\n", g_vs_para.vsp_soll[0]);// = VSP_EIN;
	}

#endif
#if 1
	//释放参数存储内存
	//Ende_VSP_Parameter();
	vs_free_parameter();
#endif
	tsc_deinit();

	return 0;
}

