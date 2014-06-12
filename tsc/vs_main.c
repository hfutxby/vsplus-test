//提供vsplus调用的入口
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "if626bas.h"
#include "if626max.h"
#include "vsp626ste.h"
#include "VSP_Interface.h"
#include "tsc.h"

typedef struct{
	VSPSollTyp vsp_soll[GERAET_TEILKNOTEN_MAX];// = {VSP_ND};
	WBTyp sg_mode[SGMAX];// = {0};
	WBReadyTyp wb_ready[GERAET_TEILKNOTEN_MAX];// = {0};
}VS_PARA;
VS_PARA g_vs_para; //VSPLUS()调用参数

xml_para* g_xml_para = NULL;

volatile int g_vsplus_ret = -1;//每1s调用一次VSPLUS()
int g_exit = 0;

pthread_t g_tid_vsplus;
int g_vsplus_exit = 0;

pthread_t g_tid_ein;
int g_ein_exit = 0;

pthread_t g_tid_aus;
int g_aus_exit = 0;

//循环调用VSPLUS()
int thr_vsplus(void* arg)
{
	struct timeval tv1, tv2;
	int ret;
	g_vsplus_exit = 0;
	while(!g_vsplus_exit){
		if(g_vsplus_ret == -1){
			printf("===VSPLUS Call===\n");
			g_vs_para.wb_ready[0] = 0;
			//for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
			//    g_vs_para.wb_ready[i] = 0;
			gettimeofday(&tv1, NULL);
			ret = VSPLUS(g_vs_para.vsp_soll, g_vs_para.sg_mode, g_vs_para.wb_ready);
			gettimeofday(&tv2, NULL);
			printf("time use: %ldus\n", (tv2.tv_sec - tv1.tv_sec)*1000000 + (tv2.tv_usec - tv1.tv_usec));
			printf("VSPLUS(%d):ret=%d\n", g_vs_para.vsp_soll[0], ret);
			//if(ret == 1){
			//    printf("%s(%d):call VSPLUS(NEU_EIN) success, ret=%d\n", __func__, __LINE__, ret);
			//}
			//else{
			//    printf("%s(%d):call VSPLUS(NEU_EIN) fail, ret=%d\n", __func__, __LINE__, ret);
			//}
			g_vsplus_ret = ret;
		}
	}
}

void* thr_aus(void* arg)
{
	g_aus_exit = 0;
	while(!g_aus_exit){
		g_vsplus_ret = -1;
		us_sleep(1000000);
		//for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.vsp_soll[0] = VSP_AUS;
	}
}

//确保每秒一次调用VSPLUS(VSP_EIN)
void* thr_ein(void* arg)
{
	g_ein_exit = 0;
	while(!g_ein_exit){//如果VSPLUS函数未能在1s内返回则关闭VSPLUS
		g_vsplus_ret = -1;
		us_sleep(1000000);//1s
		if(g_vsplus_ret == -1){
			printf("===>need switch to fix run\n");
			g_vs_para.vsp_soll[0] = VSP_AUS;
			//for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
			//    g_vs_para.vsp_soll[i] = VSP_NEU_AUS;
			pthread_create(&g_tid_aus, NULL, thr_aus, NULL);
			g_ein_exit = 1;
		}
		else{
			//for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
			g_vs_para.vsp_soll[0] = VSP_EIN;
		}
	}
}


//成功返回0
int vs_init(void)
{
	int i, ret = 0;

	g_xml_para = (xml_para*)open_xml_para();
	if(g_xml_para == NULL){
		debug(1, "open_xml_para() error\n");
		return -1;
	}
	parse_xml(g_xml_para);
	//dump_xml(g_xml_para);

	ret = tsc_init(); //控制器初始化，FIXME，分解
	if(ret == -1){
		debug(1, "tsc_init() error\n");
		return -1;
	}

#if 1
	//初始化参数存储区
	ret = vs_init_parameter();
	printf("%s(%d):vs_init_parameter ret=%d\n", __func__, __LINE__, ret);
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

#if 10
	//读参数配置文件（vcb文件）
	ret = vs_read_parameter();
	printf("%s(%d):vs_read_parameter ret=%d\n", __func__, __LINE__, ret);
	if(ret < 0)
		return -1;
#endif

#if 1
	//VSP_NEU_INI
	printf("GERAET_TEILKNOTEN_MAX:%d, SGMAX:%d\n", GERAET_TEILKNOTEN_MAX, SGMAX);
	for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.vsp_soll[i] = VSP_NEU_INI;
	for(i = 0; i < SGMAX; i++)
		g_vs_para.sg_mode[i] = 0;
	for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.wb_ready[i] = 0;
	ret = VSPLUS(g_vs_para.vsp_soll, g_vs_para.sg_mode, g_vs_para.wb_ready);
	if(ret  >= 0){
		printf("%s(%d):call VSPLUS(VSP_NEU_INI) success, ret=%d\n", __func__, __LINE__, ret);
	}
	else{
		printf("%s(%d):call VSPLUS(VSP_NEU_INI) fail, ret=%d\n", __func__, __LINE__, ret);
		return -1;
	}
#endif

#if 10
	//VSP_AUS
	for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.vsp_soll[i] = VSP_AUS;
	for(i = 0; i < SGMAX; i++)
		g_vs_para.sg_mode[i] = 0;
	for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.wb_ready[i] = 0;
	ret = VSPLUS(g_vs_para.vsp_soll, g_vs_para.sg_mode, g_vs_para.wb_ready);
	if(ret >= 0){
		printf("%s(%d):call VSPLUS(VSP_AUS) success, ret=%d\n", __func__, __LINE__, ret);
	}
	else{
		printf("%s(%d):call VSPLUS(VSP_AUS) fail, ret=%d\n", __func__, __LINE__, ret);
		return -1;
	}
#endif
	return 0;
}

//开始每秒调用一次VSPLUS(VSP_EIN)
int vs_start(void)
{
	int i, ret;
#if 1
	//关闭AUS线程
	g_aus_exit = 1;
	ret = pthread_join(g_tid_aus, NULL);
	printf("pthread_join(g_tid_aus, NULL):ret=%d\n", ret);
	g_vsplus_exit = 1;
	ret = pthread_join(g_tid_vsplus, NULL);
	printf("pthread_join(g_tid_vsplus, NULL):ret=%d\n", ret);
#endif
#if 10
	//VSP_NEU
	for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.vsp_soll[i] = VSP_NEU;
	for(i = 0; i < SGMAX; i++)
		g_vs_para.sg_mode[i] = 0;
	for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.wb_ready[i] = 0;
	ret = VSPLUS(g_vs_para.vsp_soll, g_vs_para.sg_mode, g_vs_para.wb_ready);
	if(ret >= 0){
		printf("%s(%d):call VSPLUS(VSP_NEU) success, ret=%d\n", __func__, __LINE__, ret);
	}
	else{
		printf("%s(%d):call VSPLUS(VSP_NEU) fail, ret=%d\n", __func__, __LINE__, ret);
		return -1;
	}
#endif

#if 1
	//开启EIN线程
	for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.vsp_soll[i] = VSP_EIN;
	for(i = 0; i < SGMAX; i++)
		g_vs_para.sg_mode[i] = 0;
	for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.wb_ready[i] = 0;

	g_vsplus_ret = 0;
	ret = pthread_create(&g_tid_vsplus, NULL, thr_vsplus, NULL);
	printf("pthread_create(&g_tid_vsplus):ret=%d\n", ret);
	if(ret != 0){
		return -1;
	}
	ret =pthread_create(&g_tid_ein, NULL, thr_ein, NULL);
	printf("pthread_create(&g_tid_ein):ret=%d\n", ret);
	if(ret != 0){
		g_vsplus_exit = 1;
		pthread_join(g_tid_vsplus, NULL);
		return -1;
	}
#endif

	return 0;
}

int vs_stop(void)
{
	int i, ret;
#if 1
	//关闭EIN线程
	g_ein_exit = 1;
	ret = pthread_join(g_tid_ein, NULL);
	printf("pthread_join(g_tid_ein, NULL):ret=%d\n", ret);
	g_vsplus_exit = 1;
	ret = pthread_join(g_tid_vsplus, NULL);
	printf("pthread_join(g_tid_vsplus, NULL):ret=%d\n", ret);
#endif
#if 1
	//开启EIN线程
	for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.vsp_soll[i] = VSP_AUS;
	for(i = 0; i < SGMAX; i++)
		g_vs_para.sg_mode[i] = 0;
	for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.wb_ready[i] = 0;

	g_vsplus_ret = 0;
	ret = pthread_create(&g_tid_vsplus, NULL, thr_vsplus, NULL);
	printf("pthread_create(&g_tid_vsplus):ret=%d\n", ret);
	if(ret != 0){
		return -1;
	}
	ret = pthread_create(&g_tid_aus, NULL, thr_aus, NULL);
	printf("pthread_create(&g_tid_aus):ret=%d\n", ret);
	if(ret != 0){
		g_vsplus_exit = 1;
		pthread_join(g_tid_vsplus, NULL);
		return -1;
	}
#endif

	return 0;
}

void vs_deinit(void)
{
#if 1
	vs_free_parameter();
#endif
	tsc_deinit();

}

