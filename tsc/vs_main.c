//提供vsplus调用的入口
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
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
static VS_PARA g_vs_para; //VSPLUS()调用参数

typedef struct { 
	long id; 
	unsigned short inst; 
	unsigned char KmpInd; 
	unsigned char ErgLaenge;
}pd_t;

xml_para* g_xml_para = NULL;

volatile int g_vsplus_ret = -1;//每1s调用一次VSPLUS()
int g_exit = 0;

pthread_t g_tid_vsplus;
int g_vsplus_exit = 0;

pthread_t g_tid_ein;
int g_ein_exit = 0;

pthread_t g_tid_aus;
int g_aus_exit = 0;

pthread_t g_tid_wunsch;
int g_wunsch_exit = 0;
int g_wunsch_count = 1;

int g_img_test[SGMAX] = {};//switch signal image

//循环调用VSPLUS()
int thr_vsplus(void* arg)
{
	struct timeval tv1, tv2;
	int ret;
	g_vsplus_exit = 0;
	while(!g_vsplus_exit){
		if(g_vsplus_ret == -1){
			printf("===VSPLUS Call===\n");
			//g_vs_para.wb_ready[0] = 0;
			//for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
			//    g_vs_para.wb_ready[i] = 0;
			gettimeofday(&tv1, NULL);
			ret = VSPLUS(g_vs_para.vsp_soll, g_vs_para.sg_mode, g_vs_para.wb_ready);
			if(ret >= 0){	
				printf("%s(%d):call VSPLUS() success, ret=%d\n", __func__, __LINE__, ret);
			}
			else{
				printf("%s(%d):call VSPLUS() fail, ret=%d\n", __func__, __LINE__, ret);
				return -1;
			}
			gettimeofday(&tv2, NULL);
			printf("time use: %ldus\n", (tv2.tv_sec - tv1.tv_sec)*1000*1000 + (tv2.tv_usec - tv1.tv_usec));
			printf("VSPLUS(%d):ret=%d\n", g_vs_para.vsp_soll[0], ret);
#if USE_INI
			//gettimeofday(&tv1, NULL);
			drv_add_ap();
			//gettimeofday(&tv2, NULL);
			//debug(1, "drv_add_ap use: %ldus\n", (tv2.tv_sec - tv1.tv_sec)*1000*1000 + (tv2.tv_usec - tv1.tv_usec));
#endif
			//if(ret == 1){
			//    printf("%s(%d):call VSPLUS(NEU_EIN) success, ret=%d\n", __func__, __LINE__, ret);
			//}
			//else{
			//    printf("%s(%d):call VSPLUS(NEU_EIN) fail, ret=%d\n", __func__, __LINE__, ret);
			//}
			g_vsplus_ret = ret;
		}
		else
			us_sleep(100);
	}
}

void* thr_aus(void* arg)
{
	g_aus_exit = 0;
	int i;
	while(!g_aus_exit){
		g_vsplus_ret = -1;
		us_sleep(1000*1000);
		//g_vs_para.vsp_soll[0] = VSP_AUS;
		for(i = 0; i < SGMAX; i++)
			g_vs_para.sg_mode[i] = 0;
		for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
			g_vs_para.vsp_soll[i] = VSP_AUS;
	}
}

void* thr_wunsch(void* arg)
{
	g_wunsch_exit = 0;
	g_wunsch_count = 0;
	int i;
	while(!g_aus_exit){
		printf("g_vs_para.wb_ready[0]=%d\n", g_vs_para.wb_ready[0]);
		if(!g_vs_para.wb_ready[0])
			g_wunsch_count++;
		else{
			g_wunsch_count = 0;
			g_aus_exit = 1;
			break;
		}
		g_vsplus_ret = -1;
		us_sleep(1000*1000);
		//g_vs_para.vsp_soll[0] = VSP_AUS;
		for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
			g_vs_para.vsp_soll[i] = VSP_WUNSCH_AUS_UM;
	}
}

//确保每秒一次调用VSPLUS(VSP_EIN)
void* thr_ein(void* arg)
{
	g_ein_exit = 0;
	int i;
	while(!g_ein_exit){//如果VSPLUS函数未能在1s内返回则关闭VSPLUS
		g_vsplus_ret = -1;
		us_sleep(1000*1000);//1s
#if 0
		if(g_vsplus_ret == -1){
			printf("===>VSPLUS() timeout, need switch to fix run\n");
			//g_vs_para.vsp_soll[0] = VSP_AUS;
			for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
			    g_vs_para.vsp_soll[i] = VSP_AUS;
			pthread_create(&g_tid_aus, NULL, thr_aus, NULL);
			g_ein_exit = 1;
		}
		else{
			//g_vs_para.vsp_soll[0] = VSP_EIN;
			for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
				g_vs_para.vsp_soll[i] = VSP_EIN;
		}
#else
		for(i = 0; i < SGMAX; i++)
			g_vs_para.sg_mode[i] = 0;

		for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
			g_vs_para.vsp_soll[i] = VSP_EIN;
#endif
	}
}

int tsc_init(void)
{
	int ret = 0;

	ret = drv_read_vsp_para();//读入vsp参数，在其他初始化函数之前
    if(ret == -1){
        debug(1, "call drv_vsp_para() fail\n");
        return -1;
    }

	ret = init_prg_track();//初始化prg_track
    if(ret == -1){
        debug(1, "call init_prg_track() fail\n");
        return -1;
    }

	ret = init_sg_track();//初始化sg_track
    if(ret == -1){
        debug(1, "call init_sg_track() fail\n");
		return -1;
	}

	ret = init_timers();//初始化内部定时器
	if(ret != 0){
		debug(1, "call init_timers() error\n");
		return -1;
	}

	ret = init_tsc_prg();//初始化内部prg
	if(ret != 0){
		debug(1, "init_tsc_prg() error\n");
		return -1;
	}

	ret = init_tsc_sg(); //初始化信号灯状态记录
	if(ret != 0){
		debug(1, "init_tsc_sg() error, %d\n", ret);
		return -1;
	}

	ret = init_det(); //初始化检测器信号跟踪记录
	if(ret != 0){
		debug(1, "init_det() error\n");
		return -1;
	}

	return 0;
}

void tsc_deinit(void)
{
	deinit_det();
	deinit_tsc_sg();
	deinit_tsc_prg();
	deinit_timers();
	deinit_sg_track();
	deinit_prg_track();
}

//成功返回0
int vs_init(void)
{
	debug(3, "==>\n");
	int i, ret = 0;

	ret = tsc_init(); //控制器初始化
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
	prg_track_cur_set(1);
	printf("=======prg_track_cru_set(1)=====\n");
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
	//sleep(1);
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
	debug(3, "<==\n");

	return 0;
}

//开始每秒调用一次VSPLUS(VSP_EIN)
int vs_start(void)
{
	int i, ret;
#if 1
	//关闭AUS线程
	if(g_tid_aus){
		g_aus_exit = 1;
		ret = pthread_join(g_tid_aus, NULL);
		printf("pthread_join(g_tid_aus, NULL):ret=%d\n", ret);
		g_tid_aus = 0;
	}
	if(g_tid_vsplus){
		g_vsplus_exit = 1;
		ret = pthread_join(g_tid_vsplus, NULL);
		printf("pthread_join(g_tid_vsplus, NULL):ret=%d\n", ret);
		g_tid_vsplus = 0;
	}
#endif
	sleep(1);
#if 10
	//VSP_NEU
	for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.vsp_soll[i] = VSP_NEU;
	for(i = 0; i < SGMAX; i++)
		g_vs_para.sg_mode[i] = 0;
	//g_vs_para.sg_mode[15] = 1;
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
	sleep(1);
#if 1
	//开启EIN线程
	for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.vsp_soll[i] = VSP_EIN;
	for(i = 0; i < SGMAX; i++)
		g_vs_para.sg_mode[i] = 0;
	for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.wb_ready[i] = 0;

	g_vsplus_ret = 0;//thr_vsplus暂停中
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
	if(g_tid_ein){
		g_ein_exit = 1;
		ret = pthread_join(g_tid_ein, NULL);
		printf("pthread_join(g_tid_ein, NULL):ret=%d\n", ret);
		g_tid_ein = 0;
	}
	if(g_tid_vsplus){
		g_vsplus_exit = 1;
		ret = pthread_join(g_tid_vsplus, NULL);
		printf("pthread_join(g_tid_vsplus, NULL):ret=%d\n", ret);
		g_tid_vsplus = 0;
	}
#endif
	sleep(1);
#if 1
	//开启AUS线程
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

int vs_switch(char *img)
{
	int i, ret;

	for(i = 0; i < SGMAX; i++)
		printf("%d ", img[i]);
	printf("\n");

#if 1
	//关闭EIN线程
	if(g_tid_ein){
		g_ein_exit = 1;
		ret = pthread_join(g_tid_ein, NULL);
		printf("pthread_join(g_tid_ein, NULL):ret=%d\n", ret);
		g_tid_ein = 0;
	}
	if(g_tid_vsplus){
		g_vsplus_exit = 1;
		ret = pthread_join(g_tid_vsplus, NULL);
		printf("pthread_join(g_tid_vsplus, NULL):ret=%d\n", ret);
		g_tid_vsplus = 0;
	}
#endif
	sleep(1);
#if 1
	//开启WUNSCH线程
	for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.vsp_soll[i] = VSP_WUNSCH_AUS_UM;
	for(i = 0; i < SGMAX; i++)
		g_vs_para.sg_mode[i] = img[i];

	g_vsplus_ret = 0;
	ret = pthread_create(&g_tid_vsplus, NULL, thr_vsplus, NULL);
	printf("pthread_create(&g_tid_vsplus):ret=%d\n", ret);
	if(ret != 0){
		return -1;
	}
	ret = pthread_create(&g_tid_aus, NULL, thr_wunsch, NULL);
	printf("pthread_create(&g_tid_aus):ret=%d\n", ret);
	if(ret != 0){
		g_vsplus_exit = 1;
		pthread_join(g_tid_vsplus, NULL);
		return -1;
	}
#endif
	while(1){
		if(g_wunsch_count == 0){//success
			ret = 0;
			break;
		}
		else if(g_wunsch_count > 30){//overtime
			ret = -1;
			break;
		}
		usleep(500 * 1000);
	}

	return ret;
}

void vs_deinit(void)
{
	printf("================================\n");
	int i, ret = 0;
#if 1
	if(g_tid_ein){
		g_ein_exit = 1;
		ret = pthread_join(g_tid_ein, NULL);
		printf("pthread_join(g_tid_ein, NULL):ret=%d\n", ret);
	}
	if(g_tid_aus){
		g_aus_exit = 1;
		ret = pthread_join(g_tid_aus, NULL);
		printf("pthread_join(g_tid_aus, NULL):ret=%d\n", ret);
	}
	if(g_tid_vsplus){
		g_vsplus_exit = 1;
		ret = pthread_join(g_tid_vsplus, NULL);
		printf("pthread_join(g_tid_vsplus, NULL):ret=%d\n", ret);
	}
#endif
	//sleep(1);

	////VSP_WUNSCH_AUS_UM
	//for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
	//		g_vs_para.vsp_soll[i] = VSP_WUNSCH_AUS_UM;
	//ret = VSPLUS(g_vs_para.vsp_soll, g_vs_para.sg_mode, g_vs_para.wb_ready);
	//if(ret >= 0){	
	//	printf("%s(%d):call VSPLUS() success, ret=%d\n", __func__, __LINE__, ret);
	//}
#if 1
	vs_free_parameter();
#endif
	tsc_deinit();
}

