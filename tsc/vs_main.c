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
		for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
			g_vs_para.vsp_soll[i] = VSP_AUS;
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
		for(i = 0; i < GERAET_TEILKNOTEN_MAX; i++)
			g_vs_para.vsp_soll[i] = VSP_EIN;
#endif
	}
}

int tsc_init(void)
{
	int ret = 0;
	
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
		debug(1, "init_tsc_sg() error\n");
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

//int vs_test_2(void)
//{
//	int id, inst, ret;
//	pd_t px;
//	unsigned char py[2];
//	int na, nb;
//
//	FILE* fp = fopen("all_oitd.ini", "wb+");
//	if(fp == NULL){
//		perror("can not open all_oitd.ini");
//		return -1;
//	}
//	ret = vs_read_process_data(NULL, fp);//读出vsplus库所有支持的OITD
//
//	//fclose(fp);
//	fseek(fp, 0, SEEK_SET);
//
//	typedef struct{
//		int oitd4; //oitd number
//		unsigned short range; //instance range
//		unsigned short pad;
//	}oitd;
//
//	oitd item;
//
//	while(1){
//		memset(&item, 0, sizeof(oitd));
//		ret = fread(&item, 1, sizeof(oitd), fp);
//		//printf("ret of feof: %d\n", feof(fp));
//		if(ret != sizeof(oitd)){
//			//printf("ret=%d, read data not enough\n", ret);
//			break;
//		}
//
//		na = (item.oitd4 >> 16) & 0xffff;
//		nb = item.oitd4 & 0xffff;
//		
//		if(na != 57)//只读取57.xxx
//			continue;
//
//		for(inst = 1; inst <= item.range; inst++){//分别读取instance
//			printf("%d.%d[%d]:", na, nb, inst);
//			px.id = item.oitd4;
//			px.inst = inst;
//			memset(py, 0, sizeof(py));
//			ret= vs_read_process_data(&px, &py);
//			if(ret == 0){//valid value
//				printf(" 0x%0x 0x%0x : %d\n", py[0], py[1], *(unsigned short*)py);
//			}
//			else{
//				printf("invalid value:\n");
//			}
//		}
//		if(feof(fp)){
//			//printf("end of file\n");
//			break;
//		}
//	}
//}
//
//int vs_test(void)
//{
//	typedef struct{
//		int oitd4; //四字节表示的编号
//		char sym[32]; //字符名
//		int size; //数据长度
//		int range; //范围
//	}oitd;
//
//	oitd g_oitd[] = { 
//		{(57 << 16 | 0), "OITD_VSP_VERSION", 2, 1},
//		{(57 << 16 | 1), "OITD_VSP_SYSTEM1", 2, 2},
//		{(57 << 16 | 2), "OITD_VSP_SYSTEM2", 2, 2},
//		{(57 << 16 | 3), "OITD_VSP_SYSTEM3", 2, 2},
//	};
//	printf("oitd number: %d\n", sizeof(g_oitd)/sizeof(oitd));
//
//	int id, inst, ret;
//
//	//char* vs_ocit_path(void);
//	//char *path = vs_ocit_path();
//	//printf("vs_oitd_path():%s\n", path);
//
//	pd_t px;
//	unsigned char py[2];
//	int na, nb;
//	for(id = 0; id < sizeof(g_oitd)/sizeof(oitd); id++){
//		na = (g_oitd[id].oitd4 >> 16) & 0xffff;
//		nb = g_oitd[id].oitd4 & 0xffff;
//		for(inst = 1; inst <= g_oitd[id].range; inst++){
//			px.id = g_oitd[id].oitd4;
//			px.inst = inst;
//			ret = vs_read_process_data(&px, NULL);
//			printf("%d.%d[%d]:(%s): ", na, nb, inst, g_oitd[id].sym);
//			if(ret == 0){
//				printf("instance does not exist\n");
//				continue;
//			}
//			else if( ret == 65525){
//				printf("type does not exist\n");
//				continue;
//			}
//			memset(py, 0, sizeof(py));
//			ret = vs_read_process_data(&px, py);
//			if(ret == 0){
//				printf(" 0x%0x 0x%0x : %d\n", py[0], py[1], *(unsigned short*)py);
//			}
//			else{
//				printf("invalid value:\n");
//			}
//		}
//	}
//}

typedef struct{
	int id;
	int inst;
}log_oitd;

int log_count = 0;

void title_print(void* arg, FILE* fp, int count)
{
	log_oitd* item = NULL;
	int cw = 4;//默认列宽3
	int cl = 12;//默认列长12
	int line_size = log_count * cw;
	char *tl = malloc(line_size * cl);
	memset(tl, ' ', line_size * cl);
	char *buf = malloc(log_count * cl);
	memset(buf, 0, log_count * cl);

	int i, j;
	for(i = 0; i < log_count; i++){
		item = (arg + i * sizeof(log_oitd));
		sprintf((buf + i * cl), "%3d.%3d-%4d", (item->id >> 16) & 0xffff, item->id & 0xffff, item->inst);
	}
	//printf("%s\n", buf);
	for(i = 0; i < cl; i++){
		for(j = 0; j < log_count; j++){
			*(tl + i * line_size + j * cw) = *(buf + j * cl + i);
			if(j == log_count -1)
				*(tl + (i + 1) * line_size - 1) = '\n'; 
		}
	}
	//for(i = 0; i < line_size * cl; i++){
	//	printf("%c ", *(tl + i));
	//}
	fwrite(tl, line_size * cl, 1, fp);
	fprintf(fp, "\n\n");
	fflush(fp);
}

void* thr_log(void* arg)
{
	FILE* fp = fopen("log.output", "wb+");
	if(fp == NULL){
		printf("fopen log.output error:%s\n", strerror(errno));
		return;
	}

	title_print(arg, fp, log_count);

	log_oitd* item = NULL;

	pd_t px;
	int i, ret;
	unsigned char py[2];
	while(!g_exit){
		for(i = 0; i < log_count; i++){
			item = arg + i*sizeof(log_oitd);
			px.id = item->id;
			px.inst = item->inst;
			//printf("id:%ld, inst:%d\n", px.id, px.inst);
			memset(py, 0, sizeof(py));
			ret = vs_read_process_data(&px, py);
			if(ret == 0)
				//fprintf(fp, "%ld.%ld[%d]:%-3d ", (px.id >> 16) & 0xffff, px.id & 0xffff, px.inst, *(unsigned short*)py);
				fprintf(fp, "%-3d ", *(unsigned short*)py);
			else
				fprintf(fp, "#   ");//invalid value
		}
		fprintf(fp, "\n");
		fflush(fp);
		us_sleep(1000000);
	}
}

int vs_log(void)
{
	int id, inst, ret;
	pd_t px;
	unsigned char py[2];
	int na, nb;
	int len;

	
	log_oitd* item = NULL;

	FILE* fp = fopen("log.def", "rb");
	if(fp == NULL){
		printf("fopen log.def error:%s\n", strerror(errno));
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	log_count = len / sizeof(log_oitd);
	printf("file log.def len:%d, items:%d\n", len, log_count);
	fclose(fp);

	int fd = open("log.def", O_RDWR);
	if(fd < 0){
		printf("open log.def error:%s\n", strerror(errno));
		return -1;
	}
	void *ptr;
	ptr = mmap(NULL, len,  PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr ==  MAP_FAILED){
        printf("mmap error:%s\n", strerror(errno));
		return -1;
    }

	pthread_t tid_log;
	pthread_create(&tid_log, NULL, thr_log, ptr);
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

