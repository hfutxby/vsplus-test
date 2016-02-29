//提供vsplus调用的入口
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include "if626bas.h"
#include "if626max.h"
#include "vsp626ste.h"
#include "VSP_Interface.h"
#include "tsc.h"


VS_PARA g_vs_para; //VSPLUS()调用参数

//xml_para* g_xml_para = NULL;

/* thread use to call VSPLUS() every seconds */
pthread_t g_tid_vsplus;
pthread_t g_tid_check_vsplus_timeout;
volatile int g_vsplus_exit = 0; //exit vsplus() call cycle
volatile int g_vsplus_timeout = 0; //vsplus() call timeout, should exit
volatile int g_vsplus_call = 0; //after 1 second, call vsplus() again
struct timeval g_vsplus_call_begin, g_vsplus_call_end; //timestamp of vsplus() call


int g_img_test[SGMAX] = { }; //switch signal image

//int g_vsplus_stat = 0; //0-aus, 1-neu
int g_vsplus_ctl_active = 1; // when off or during switch on or switch off, return 1;

//make sure vsplus() call every seconds and finished in 1 sec
void* thr_check_vsplus_timeout(void* arg)
{
	struct timeval now;
	int diff = 0;
	while (!g_vsplus_exit) {
		gettimeofday(&now, NULL);
		diff = (now.tv_sec - g_vsplus_call_begin.tv_sec) * 1000
				+ (now.tv_usec - g_vsplus_call_begin.tv_usec) / 1000;
		if (diff >= 1000) { //1 sec
			if (timercmp(&g_vsplus_call_begin, &g_vsplus_call_end, <)) { //vsplus() call has finished in 1 sec
				g_vsplus_call = 1; //allow next call to vsplus()
			} else {
				printf("%s(%d):call VSPLUS() timeout %dms\n", __func__,
				__LINE__, diff);
//				g_vsplus_timeout = 1; // TODO  vsplus() call exceed 1 sec,
				g_vsplus_exit = 1; //exit all vsplus threads
			}
		}
		us_sleep(10 * 1000);
	}
	pthread_exit(NULL);
}

//循环调用VSPLUS()
int thr_vsplus(void* arg)
{
	int ret;
	g_vsplus_call = 1;
	gettimeofday(&g_vsplus_call_begin, NULL);
	gettimeofday(&g_vsplus_call_end, NULL);
	pthread_create(&g_tid_check_vsplus_timeout, NULL, thr_check_vsplus_timeout,
	NULL);
	while (!g_vsplus_exit) {
		if (g_vsplus_call) {
			g_vsplus_call = 0;
			gettimeofday(&g_vsplus_call_begin, NULL);
			ret = VSPLUS(g_vs_para.vsp_soll, g_vs_para.sg_mode,
					g_vs_para.wb_ready);
			gettimeofday(&g_vsplus_call_end, NULL);
			if (ret < 0) {
				printf("%s(%d):call VSPLUS() fail, ret=%d\n", __func__,
				__LINE__, ret);
				g_vsplus_exit = 1;
				pthread_exit(NULL);
			}
#if USE_INI
			drv_add_ap();
#endif
		} else
			us_sleep(10 * 1000);
	}
}

int tsc_init(void)
{
	int ret = 0;

	ret = drv_read_vsp_para();			//读入vsp参数，在其他初始化函数之前
	if (ret == -1) {
		debug(1, "call drv_vsp_para() fail\n");
		return -1;
	}

	ret = init_prg_track();			//初始化prg_track
	if (ret == -1) {
		debug(1, "call init_prg_track() fail\n");
		return -1;
	}

	ret = init_sg_track();			//初始化sg_track
	if (ret == -1) {
		debug(1, "call init_sg_track() fail\n");
		return -1;
	}

	ret = init_timers();			//初始化内部定时器
	if (ret != 0) {
		debug(1, "call init_timers() error\n");
		return -1;
	}

	ret = init_tsc_prg();			//初始化内部prg
	if (ret != 0) {
		debug(1, "init_tsc_prg() error\n");
		return -1;
	}

	ret = init_tsc_sg(); //初始化信号灯状态记录
	if (ret != 0) {
		debug(1, "init_tsc_sg() error, %d\n", ret);
		return -1;
	}

	ret = init_det(); //初始化检测器信号跟踪记录
	if (ret != 0) {
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
	int i, ret = 0;

	ret = tsc_init(); //控制器初始化
	if (ret == -1) {
		debug(1, "tsc_init() error\n");
		return -1;
	}

#if 1
	//初始化参数存储区
	ret = vs_init_parameter();
	printf("%s(%d):vs_init_parameter ret=%d\n", __func__, __LINE__, ret);
	if (ret < 0) {
		return -1;
	}
#endif

#if 1
	//检查参数配置文件（vcb文件）
	ret = vs_chk_parameter();
	printf("%s(%d):vs_chk_parameter ret=%d\n", __func__, __LINE__, ret);
	if (ret < 0) {
		return -1;
	}
#endif

#if 10
	//读参数配置文件（vcb文件）
	ret = vs_read_parameter();
	printf("%s(%d):vs_read_parameter ret=%d\n", __func__, __LINE__, ret);
	if (ret < 0) {
		return -1;
	}
#endif

	prg_track_cur_set(1); //FIXME
	printf("=======prg_track_cru_set(1)=====\n");

#if 1
	//VSP_NEU_INI
	printf("GERAET_TEILKNOTEN_MAX:%d, SGMAX:%d\n", GERAET_TEILKNOTEN_MAX,
	SGMAX);
	for (i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.vsp_soll[i] = VSP_NEU_INI;
	for (i = 0; i < SGMAX; i++)
		g_vs_para.sg_mode[i] = 0;
	for (i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.wb_ready[i] = 0;
	ret = VSPLUS(g_vs_para.vsp_soll, g_vs_para.sg_mode, g_vs_para.wb_ready);
	if (ret >= 0) {
		printf("%s(%d):call VSPLUS(VSP_NEU_INI) success, ret=%d\n", __func__,
		__LINE__, ret);
	} else {
		printf("%s(%d):call VSPLUS(VSP_NEU_INI) fail, ret=%d\n", __func__,
		__LINE__, ret);
		return -1;
	}
#endif

	//sleep(1);
#if 10
	//VSP_AUS
	for (i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.vsp_soll[i] = VSP_AUS;
	for (i = 0; i < SGMAX; i++)
		g_vs_para.sg_mode[i] = 0;
	for (i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.wb_ready[i] = 0;
	ret = VSPLUS(g_vs_para.vsp_soll, g_vs_para.sg_mode, g_vs_para.wb_ready);
	if (ret >= 0) {
		printf("%s(%d):call VSPLUS(VSP_AUS) success, ret=%d\n", __func__,
		__LINE__, ret);
	} else {
		printf("%s(%d):call VSPLUS(VSP_AUS) fail, ret=%d\n", __func__, __LINE__,
				ret);
		return -1;
	}
#endif

#if 1
	pthread_create(&g_tid_vsplus, NULL, thr_vsplus, NULL);
//	pthread_create(&g_tid_check_vsplus_timeout, NULL, thr_check_vsplus_timeout,	NULL);
#endif

	return 0;
}

//开始每秒调用一次VSPLUS(VSP_EIN)
int vs_start(void)
{
	int i, ret;

	for (i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.vsp_soll[i] = VSP_NEU;
	for (i = 0; i < SGMAX; i++)
		g_vs_para.sg_mode[i] = 0;
	for (i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.wb_ready[i] = 0;

	sleep(1);

	for (i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.vsp_soll[i] = VSP_EIN;
	for (i = 0; i < SGMAX; i++)
		g_vs_para.sg_mode[i] = 0;
	for (i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.wb_ready[i] = 0;

	g_vsplus_ctl_active = 0;

	sleep(1);

	return 0;
}

int vs_stop(void)
{
	int i, ret;

	for (i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.vsp_soll[i] = VSP_AUS;
	for (i = 0; i < SGMAX; i++)
		g_vs_para.sg_mode[i] = 0;
	for (i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
		g_vs_para.wb_ready[i] = 0;

	g_vsplus_ctl_active = 1;

	sleep(1);

	return 0;
}

void vs_deinit(void)
{

	int i, ret = 0;

//	if (g_tid_vsplus) {
//		g_vsplus_exit = 1;
//		ret = pthread_join(g_tid_vsplus, NULL);
//		printf("pthread_join(g_tid_vsplus, NULL):ret=%d\n", ret);
//	}

#if 1
	vs_free_parameter();
#endif
	tsc_deinit();
}

