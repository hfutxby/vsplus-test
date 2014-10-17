//驱动板功能
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <limits.h>

#include "drive.h"
#include "tsc.h"
#include "serial_pack.h"
#include "vcb.h"
#include "if626bas.h"

//vsplus信号灯编号与底板实际信号灯编号转换
int vcb_sg_code[] = {0,
  0x11,//sg1
  0x12,//sg2
  0x13,//sg3
  0x14,//sg4
  0x21,//sg5
  0x22,//sg6
  0x23,//sg7
  0x24,//sg8
  0x41,//sg13
  0x42,//sg14
  0x43,//sg15
  0x44,//sg15
  0x61,//sg21
  0x62,//sg22
  0x63,//sg23
  0x64//sg24 
};

//vsplus信号灯状态和底板信号灯状态对应关系
int vcb_sg_stat_code[10] = {
  0x00, //0:dark
  0x02, //1:amber
  0x01, //2:min-red
  0x01, //3:ext-red
  0x02, //4:prep
  0x04, //5:min-green
  0x04, //6:ext-green
  0x14, //7:绿闪
  0x11, //8:红闪
  0x12, //9:黄闪
};

//取得vsplus定义的信号灯在信号机控制器中的编号
int drv_sg_code(int sg)
{
	if(sg < (sizeof(vcb_sg_code) / sizeof(int)))
		return vcb_sg_code[sg];
	else
		return 0;
}

//取得vsplus信号灯状态在信号机控制器中的编码
int drv_sg_stat_code(int stat)
{
	if(stat < (sizeof(vcb_sg_stat_code) / sizeof(int)))
		return vcb_sg_stat_code[stat];
	else
		return -1;
}

//切换信号灯并记录状态
void drv_sg_switch(int sg, int stat)
{
	unsigned char msg[4];
	msg[0] = 0x96; msg[3] = 0x69;
	msg[1] = drv_sg_code(sg);
	msg[2] = drv_sg_stat_code(stat);
	serial_command(msg, 4);//通过串口切换信号灯
	sg_track_switch(sg, stat);//记录信号灯状态信息	
#if USE_INI
	drv_add_sg(sg, stat);
#endif/* USE_INI */
}

//只发送串口命令切换信号灯
void drv_sg_switch2(int sg, int stat)
{
	unsigned char msg[4];
	msg[0] = 0x96; msg[3] = 0x69;
	msg[1] = drv_sg_code(sg);
	msg[2] = drv_sg_stat_code(stat);
	serial_command(msg, 4);//通过串口切换信号灯
#if USE_INI
	drv_add_sg(sg, stat);
#endif/* USE_INI */

}

//获取信号灯配置
//ptr为sg_def结构
int drv_sg_para(void* ptr, int size)
{
#if 0
	int xml_sg_para(void* ptr, int size);
#else
	int i;
	memset(ptr, 0, size);
	int num = size / sizeof(sg_def);
	int num_a = sizeof(vcb_sg_exist) / sizeof(int);
	num = (num > num_a) ? num_a : num;
	sg_def* sg = NULL;
	for(i = 0; i < num; i++){
		sg = (sg_def*)(ptr + i * sizeof(sg_def));
		sg->min_red = vcb_close_sum[i];
		sg->min_green = vcb_free_sum[i];
		sg->prep = vcb_prep[i];
		sg->amber = vcb_amber[i];
		sg->green_blink = vcb_green_blink[i];
		sg->ext = 0;
		sg->exist = vcb_sg_exist[i];
	}

	return num;
#endif
}

//打印信号灯配置
void drv_sg_para_dump(void* ptr, int size)
{
	int i;
	int num = size / sizeof(sg_def);
	sg_def* sg = NULL;
	printf("===%s===\n", __func__);
	printf("%9s %9s %9s %9s\n", "sg_id", "min_red", "min_green", "prep");
	for(i = 0; i < num; i++){
		sg = (sg_def*)(ptr + i * sizeof(sg_def));
		if(sg->exist)
			printf("%9d %9d %9d %9d\n", i, sg->min_red, sg->min_green, sg->prep);
	}
}

//获取检测器配置
int drv_det_para(void* ptr, int size)
{
#if 0
	int xml_det_para(void* ptr, int size);
#else
	memset(ptr, 0, size);
	int i;
	det_def* det = NULL;
	int num = size / sizeof(det_def);
	int num_a = sizeof(vcb_det_exist) / sizeof(int);
	num = (num > num_a) ? num_a : num;
	debug(3, "num:%d\n", num);
	for(i = 0; i < num; i++){
		det = (det_def*)(ptr + i * sizeof(det_def));
		det->exist = vcb_det_exist[i];
	}
	return 0;
#endif
}

/*=== 串口命令 ===*/
//处理串口命令
int drv_handle_pack(unsigned char* buf)
{
	printf("command: 0x%x\n", buf[0]);

	return 0;
}

//获取配时方案配置
//prg_def参数
int drv_prg_para(void* ptr, int size)
{
#if 0
	int xml_prg_para(void* ptr, int size);
#else
	memset(ptr, 0, size);
	int i;
	prg_def* prg = NULL;
	int num = size / sizeof(prg_def);
	int num_a = sizeof(vcb_prg_exist) / sizeof(int);
	num = (num > num_a) ? num_a : num;
	debug(3, "ptr:%p, size:%d, num:%d\n", ptr, size, num);
	for(i = 0; i < num; i++){
		prg = (prg_def*)(ptr + i * sizeof(prg_def));
		prg->exist = vcb_prg_exist[i];
		prg->tu = vcb_prg_tu[i];
	}
	return 0;
#endif
}

void drv_prg_para_dump(void* ptr, int size)
{
	int num = size / sizeof(prg_def);
	int i;
	prg_def* prg = NULL;
	printf("===%s===\n", __func__);
	printf("%6s %6s\n", "prg_id", "tu");
	for(i = 0; i < num; i++){
		prg = (prg_def*)(ptr + i * sizeof(prg_def));
		if(prg->exist)
			printf("%6d %6d\n", i, prg->tu);
	}
}

//设置下一个配时方案
int drv_prg_next_set(int index)
{
	return prg_track_next_set(index);
}

//获得绿间隔配置
int drv_inter_green(int sgr, int sge)
{
	//int num = sizeof(vcb_inter_green) / (sizeof(int)*3);
	//int i;
	//for(i = 0; i < num; i++){
	//	if((vcb_inter_green[i][0] == sgr) && (vcb_inter_green[i][1] == sge)){
	//		return vcb_inter_green[i][2];
	//	}
	//}

	return vcb_inter_green[sgr-1][sge-1];
}

//获得ocit编号配置
int drv_get_ocitid(int* ZNr, int* FNr, int*Realknoten)
{
	*ZNr = vcb_ZNr;
	*FNr = vcb_FNr;
	*Realknoten = vcb_Realknoten;

	return 1;
}

int drv_read_vsp_para(void)
{
	int i, j, len;
	int ret = 0;

	//read
	FILE* fp = fopen(VSP_PARAMFILE, "rb");
	if(fp == NULL){
		debug(1, "fopen %s error\n", VSP_PARAMFILE);
		return -1;
	}

	if(fread(&VSPSysData, sizeof(VSPSysData), 1, fp) <= 0)
	{
		debug(1, "fread error\n");
		fclose(fp);
		ret = -1;
	}

	if(fread(&VSPDetData, sizeof(VSPDetData), 1, fp) <= 0)
	{
		debug(1, "fread error\n");
		fclose(fp);
		ret = -1;
	}

	if(fread(&VSPSigData, sizeof(VSPSigData), 1, fp) <= 0)
	{
		debug(1, "fread error\n");
		fclose(fp);
		ret = -1;
	}

	fclose(fp);
	//vcb_sg_code[]
#if 0
	len = sizeof(vcb_sg_code) / sizeof(int);
	printf("len:%d\n", len);
	printf("%2s  %2s\n", "sg", "code");
	for(i = 0; i < len; i++){
		printf("%2d  0x%02x\n", i, vcb_sg_code[i]);
	}
	printf("\n");

	//vcb_sg_stat_code
	len = sizeof(vcb_sg_stat_code) / sizeof(int);
	printf("len:%d\n", len);
	printf("%2s  %2s\n", "sg_stat", "code");
	for(i = 0; i < len; i++){
		printf("%2d  0x%02x\n", i, vcb_sg_stat_code[i]);
	}
	printf("\n");
#endif

	//vcb_sg
	for(i = 0; i < MAX_SIG_GROUP; i++){
		vcb_sg_exist[i+1] = VSPSigData.VSPSigDataList[i].sig_valid;
		vcb_free_sum[i+1] = VSPSigData.VSPSigDataList[i].mingreen_time;
		vcb_close_sum[i+1] = VSPSigData.VSPSigDataList[i].minred_time;
		vcb_amber[i+1] = VSPSigData.VSPSigDataList[i].amber_time;
		vcb_prep[i+1] = VSPSigData.VSPSigDataList[i].prep_time;
		vcb_green_blink[i+1] = VSPSigData.VSPSigDataList[i].green_blink;
	}
#if 0
	printf("%2s  %5s  %4s  %5s  %5s  %4s  %10s\n", "sg", "exist", "free", "close", "amber", "prep", "greenblink");
	len = sizeof(vcb_sg_exist) / sizeof(int);
	printf("len:%d\n", len);
	for(i = 0; i < MAX_SIG_GROUP+1; i++){
		printf("%2d  %5d  %4d  %5d  %5d  %4d  %10d\n", i, vcb_sg_exist[i], vcb_free_sum[i],
				vcb_close_sum[i], vcb_amber[i], vcb_prep[i], vcb_green_blink[i]);
	}
	printf("\n");
#endif
	//vcb_inter_green
	for(i = 0; i < MAX_SIG_GROUP; i++){
		for(j = 0; j < MAX_SIG_GROUP; j++)
			vcb_inter_green[i][j] = VSPSysData.inter_green[i][j];
	}
#if 0
	printf("%3s  %3s  %3s\n", "sgr", "sge", "val");
	for(i = 0; i < MAX_SIG_GROUP; i++){
		for(j = 0; j < MAX_SIG_GROUP; j++)
			if(vcb_inter_green[i][j])
				printf("%3d  %3d  %3d\n", i+1, j+1, vcb_inter_green[i][j]);
	}
#endif

	//vcb_det_exist
	memset(&vcb_det_exist, 0, sizeof(int)* (DETMAX+1));
	for(i = 0; i < MAX_DET_GROUP; i++){
		vcb_det_exist[i+1] = VSPDetData.VSPDetDataList[i].det_valid;
	}
#if 0
	printf("%3s  %5s\n", "det", "valid");
	for(i = 0; i < MAX_DET_GROUP+1; i++){
		if(vcb_det_exist[i])
			printf("%3d  %5d\n", i, vcb_det_exist[i]);
	}
	printf("\n");
#endif

	//vcb_prg_exist
	for(i = 0; i < MAX_VSP_PROG; i++){
		vcb_prg_exist[i+1] = (VSPSysData.vsp_cycletime[i] > 0) ? 1 : 0;
		vcb_prg_tu[i+1] = VSPSysData.vsp_cycletime[i];
	}
#if 0
	printf("%3s  %5s  %2s\n", "prg", "exist", "tu");
	for(i = 0; i < MAX_VSP_PROG+1; i++){
		printf("%3d  %5d  %2d\n", i, vcb_prg_exist[i], vcb_prg_tu[i]);
	}
	printf("\n");
#endif
	//ID
	vcb_ZNr = VSPSysData.area_num;
	vcb_FNr = VSPSysData.con_num;
	vcb_Realknoten = VSPSysData.node_num;
#if 1
	printf("ZNr:%d\nFNr:%d\nRealknoten:%d\n", vcb_ZNr, vcb_FNr, vcb_Realknoten);
#endif

	return ret;
}


/* ========== 输出ini文件 ============== */
//打印det状态
static char *ptr_det = NULL;
static int g_size_det = 0;
static int g_space_det = 0;
static struct timeval g_tv_det = {};
static int g_dir_det = 1;
#define LOGBAK 1

void free_det(void)
{
	if(ptr_det != NULL){
		free(ptr_det);
		ptr_det = NULL;
	}
	g_dir_det = 1;
}

int drv_add_det(int det, int value)
{
//debug(1, "det:%d, value:%d\n", det, value);
	//首次确认目录存在
	if(g_dir_det){
		if(access("log", F_OK) == -1){
			if(mkdir("log", 0777) == -1)
				perror("mkdir log");
		}
#ifdef LOGBAK
		if(access("log_bak", F_OK) == -1){
			if(mkdir("log_bak", 0777) == -1)
				perror("mkdir log_bak");
		}
#endif
		//if(access("log/det", F_OK) == -1){
		//	if(mkdir("log/det", 0777) == -1)
		//		perror("mkdir log/det");
		//}
		g_dir_det = 0;
		g_size_det = 1024;
		g_space_det = g_size_det;
		ptr_det = malloc(g_size_det);//首次分配空间,FIXME:泄露风险
		memset(ptr_det, 0, g_size_det);
	}

	char str[256] = {};
	struct timeval tv;
	gettimeofday(&tv, NULL);
	struct tm st = {0};
	localtime_r(&tv.tv_sec, &st);

	if(g_tv_det.tv_sec != tv.tv_sec){
		if(g_tv_det.tv_sec != 0){//保存文件
			struct tm gst = {0};
			localtime_r(&g_tv_det.tv_sec, &gst);
			memset(str, 0, sizeof(str));
			sprintf(str, "log/%d_%04d%02d%02d%02d%02d%02d_det.ini",
					vcb_FNr, gst.tm_year+1900, gst.tm_mon+1, 
					gst.tm_mday, gst.tm_hour, gst.tm_min, 
					gst.tm_sec);
			FILE* fp = fopen(str, "wb");
			if(fp == NULL){
				debug(1, "open file %s error\n", str);
				return -1;
			}
			fwrite(ptr_det, g_size_det - g_space_det, 1, fp);
			fclose(fp);
#ifdef LOGBAK
			memset(str, 0, sizeof(str));
			sprintf(str, 
					"log_bak/%d_%04d%02d%02d%02d%02d%02d_det.ini",
					vcb_FNr, gst.tm_year+1900, gst.tm_mon+1, 
					gst.tm_mday, gst.tm_hour, gst.tm_min, 
					gst.tm_sec);

			fp = fopen(str, "wb");
			if(fp == NULL){
				debug(1, "open file %s error\n", str);
				return -1;
			}
			fwrite(ptr_det, g_size_det - g_space_det, 1, fp);
			fclose(fp);
#endif
			memset(ptr_det, 0, g_size_det);
			g_space_det = g_size_det;
		}
		g_tv_det.tv_sec = tv.tv_sec;
		g_tv_det.tv_usec = tv.tv_usec;
	}

	memset(str, 0, sizeof(str));
	int diff = (tv.tv_usec/100000 - g_tv_det.tv_usec/100000)*100;
	sprintf(str, "[%d_D_%d %d]\r\n", vcb_FNr, det, diff);
	sprintf(str+strlen(str), "Value=%d\r\n", value);
	sprintf(str+strlen(str), 
			"Time=%04d%02d%02d%02d%02d%02d%03ld\r\n", 
			st.tm_year+1900, st.tm_mon+1, st.tm_mday, st.tm_hour, 
			st.tm_min, st.tm_sec, (tv.tv_usec/100000)*100);
	//printf("%s", str);

	if(g_space_det < strlen(str)){//扩展存储区
		debug(2, "g_space_det:%d, strlen:%d\n", g_space_det, strlen(str));
		char* ptr = malloc(g_size_det+1024);
		memset(ptr, 0, g_size_det+1024);
		memcpy(ptr, ptr_det, strlen(ptr_det));
		free(ptr_det);
		ptr_det = ptr;
		ptr = NULL;
		g_size_det += 1024;
		g_space_det += 1024;
	}	
	strcat(ptr_det, str);
	g_space_det -= strlen(str);
	return 0;
}


//打印sg状态
static char *ptr_sg = NULL;//写入文件前数据
static int g_size_sg = 0;
static int g_space_sg = 0;
static struct timeval g_tv_sg = {};//每秒首次产生记录时的时间
static int g_dir_sg = 1;//首次调用标志

void free_sg(void)
{
	if(ptr_sg != NULL){
		free(ptr_sg);
		ptr_sg = NULL;
	}
	g_dir_sg = 1;
}

int drv_add_sg(int sg, int stat)
{
	//首次确认目录存在
	if(g_dir_sg){
		if(access("log", F_OK) == -1){
			if(mkdir("log", 0777) == -1)
				perror("mkdir log");
		}
#ifdef LOGBAK
		if(access("log_bak", F_OK) == -1){
			if(mkdir("log_bak", 0777) == -1)
				perror("mkdir log_bak");
		}
#endif
		g_dir_sg = 0;
		g_size_sg = 1024;
		g_space_sg = g_size_sg;
		ptr_sg = malloc(g_size_sg);//首次分配空间,FIXME:泄露风险
		memset(ptr_sg, 0, g_size_sg);
	}

	//ext red, ext green实际没有产生灯色变化
	if((stat == 3) || (stat == 6))
		return 0;

	int value;
	//[7],always=0;
	//[6],blink Hz, 0:1Hz, 1:2Hz
	//[5:4],green mode, 11b:green, 01:dark-green, 10:green-dark
	//[3:2],yellow mode, [1:0],red mode
	switch(stat){
		case 1://amber
			value = 0b00001100;//12
			break;
		case 2://min red
			value = 0b00000011;//3
			break;
		case 4://prep
			value = 0b00001111;//15
			break;
		case 5://min greeen
			value = 0b00110000;//48
			break;
		case 7://green blink
			value = 0b0010000;//16
			break;
	}

	char str[256] = {};
	struct timeval tv;
	gettimeofday(&tv, NULL);
	struct tm st = {0};
	localtime_r(&tv.tv_sec, &st);
	//printf("Time=%04d%02d%02d%02d%02d%02d%03ld\r\n", 
	//		st.tm_year+1900, st.tm_mon+1, st.tm_mday, st.tm_hour, 
	//		st.tm_min, st.tm_sec, (tv.tv_usec / 100000)*100);

	if(g_tv_sg.tv_sec != tv.tv_sec){
		if(g_tv_sg.tv_sec != 0){//保存文件
			struct tm gst = {0};
			localtime_r(&g_tv_sg.tv_sec, &gst);
			FILE* fp = NULL;
			memset(str, 0, sizeof(str));
			sprintf(str, "log/%d_%04d%02d%02d%02d%02d%02d_sg.ini", 
					vcb_FNr, gst.tm_year+1900, gst.tm_mon+1, 
					gst.tm_mday, gst.tm_hour, gst.tm_min, 
					gst.tm_sec);
			fp = fopen(str, "wb");
			if(fp == NULL){
				debug(1, "open file %s error\n", str);
				return -1;
			}
			fwrite(ptr_sg, g_size_sg - g_space_sg, 1, fp);
			fclose(fp);
#ifdef LOGBAK
			memset(str, 0, sizeof(str));
			sprintf(str, 
					"log_bak/%d_%04d%02d%02d%02d%02d%02d_sg.ini", 
					vcb_FNr, gst.tm_year+1900, gst.tm_mon+1, 
					gst.tm_mday, gst.tm_hour, gst.tm_min, 
					gst.tm_sec);

			fp = fopen(str, "wb");
			if(fp == NULL){
				debug(1, "open file %s error\n", str);
				return -1;
			}
			fwrite(ptr_sg, g_size_sg - g_space_sg, 1, fp);
			fclose(fp);
#endif
			memset(ptr_sg, 0, g_size_sg);
			g_space_sg = g_size_sg;
		}
		g_tv_sg.tv_sec = tv.tv_sec;
		g_tv_sg.tv_usec = tv.tv_usec;
	}

	memset(str, 0, sizeof(str));
	int diff = (tv.tv_usec/100000 - g_tv_sg.tv_usec/100000) * 100;//100ms
	sprintf(str, "[%d_S_%d %d]\r\n", vcb_FNr, sg, diff);
	sprintf(str+strlen(str), "Value=%d\r\n", value);
	sprintf(str+strlen(str), 
			"Time=%04d%02d%02d%02d%02d%02d%03ld\r\n", 
			st.tm_year+1900, st.tm_mon+1, st.tm_mday, st.tm_hour, 
			st.tm_min, st.tm_sec, (tv.tv_usec / 100000)*100);

	if(g_space_sg < strlen(str)){//扩展存储区
		char* ptr = malloc(g_size_sg+1024);
		memset(ptr, 0, g_size_sg+1024);
		memcpy(ptr, ptr_sg, strlen(ptr_sg));
		free(ptr_sg);
		ptr_sg = ptr;
		ptr = NULL;
		g_size_sg += 1024;
		g_space_sg += 1024;
	}	
	//printf("strlen(ptr_sg):%d, t1:%ld, t2:%ld\n str:\n%s\n", strlen(ptr_sg), g_tv_sg.tv_sec, tv.tv_sec, str);
	strcat(ptr_sg, str);
	g_space_sg -= strlen(str);
	return 0;
}

#if 1
//打印ap
typedef struct {
    long id;
    unsigned short inst;
    unsigned char KmpInd;
    unsigned char ErgLaenge;
}pd_t;

typedef struct {
    long id;
    unsigned short inst;
    unsigned short val;
}ap_t;

static int g_dir_ap = 1; //首次调用时检查日志目录是否存在
static ap_t* g_ptr_ap_all = NULL; //所有使用vs_read_process_data()可能获得的ap
static int g_ap_all_num = 0;
static ap_t* g_ptr_ap_def = NULL; //需要检查的ap
static int g_ap_def_num = 0;
static int g_ap_def_inst = 0;
static char* g_ptr_ap_diff = NULL; //输出变化ap的ini记录
static int g_ap_diff_size = 0; 
static struct timeval g_tv_ap = {}; //

#define INC_SIZE 4096 //动态内存增加

void free_ap(void)
{
	if(g_ptr_ap_all){
		free(g_ptr_ap_all);
		g_ptr_ap_all = NULL;
	}
	if(g_ptr_ap_def){
		free(g_ptr_ap_def);
		g_ptr_ap_def = NULL;
	}
	if(g_ptr_ap_diff){
		free(g_ptr_ap_diff);
		g_ptr_ap_diff = NULL;
	}
	g_dir_ap = 1;
}

int getid(char* line)
{
    int a, b, i;
    int size = strlen(line);
    a = atoi(line);
    for(i = 0; i < size; i++){
        if(line[i] == '.')
            break;
    }
    if(i != (size-1))
        b = atoi(line+i+1);
    else
        b = 0;

    return a << 16 | b;
}

int getinst(int id)
{
    int i;
    for(i = 0; i < g_ap_all_num; i++){
        if(g_ptr_ap_all[i].id == id)
            return g_ptr_ap_all[i].inst;
    }

    return 0;
}

int drv_add_ap(void)
{
	int i, j, ret;
	//确认目录存在
	if(g_dir_ap){
		g_dir_ap = 0;
		if(access("log", F_OK) == -1){
			mkdir("log", 0777);
		}
#ifdef LOGBAK
		if(access("log_bak", F_OK) == -1){
			mkdir("log_bak", 0777);
		}
#endif
		//所有可能存在的ap
		FILE* fp = fopen("ap.dat", "wb");
		ret = vs_read_process_data(NULL, fp);
		int file_size = ftell(fp);
		g_ap_all_num = file_size / sizeof(pd_t);
		fclose(fp);
		fp = fopen("ap.dat", "rb");
		g_ptr_ap_all = (ap_t*)malloc(file_size);
		ret = fread(g_ptr_ap_all, 1, file_size, fp);
		fclose(fp);
		remove("ap.dat");
		//for(i = 0; i < 16; i++)//TEST
		//	printf("0x%x ", *((char*)(g_ptr_ap_all) + i));
		//printf("\n");

		//分配存储ap值的内存区域
		fp = fopen("ap.def", "rb");
		if(fp == NULL){
			debug(1, "open file ap.def error\n");
			fp = fopen("ap.def", "wb");
			char str[] = "57.0\r\n";//default
			fwrite(str, sizeof(str), 1, fp);
			fclose(fp);
			fp = fopen("ap.def", "rb");
		}
		char *line = NULL;
		int size, n;
		int inst, id;
		while((size = getline(&line, &n, fp)) != -1){
			g_ap_def_num++;
			id = getid(line);
			g_ap_def_inst += getinst(id);
		}
		fclose(fp);
		//debug(1, "ap.def, ap_num:%d, ap_inst:%d\n", g_ap_def_num, g_ap_def_inst);//TEST

		size = g_ap_def_inst * sizeof(ap_t);
		g_ptr_ap_def = (ap_t*)malloc(size);
		memset(g_ptr_ap_def, 0, size);

		fp = fopen("ap.def", "rb");
		if(fp == NULL){
			debug(1, "open file ap.def error\n");
		}
		i = 0;
		while((size = getline(&line, &n, fp)) != -1){
			id = getid(line);
			inst = getinst(id);
			//debug(1, "%s, id:%d, inst:%d", line, id, inst);
			for(j = 1; j <= inst; j++){
				g_ptr_ap_def[i].id = id;
				g_ptr_ap_def[i].inst = j;
				g_ptr_ap_def[i].val = 0;
				i++;
			}
		}
		fclose(fp);
		//for(i = 0; i < 3; i++){//TEST
		//	debug(1, "id:%ld.%ld inst:%d val:%d\n", g_ptr_ap_def[i].id >> 16 & 0xffff, g_ptr_ap_def[i].id & 0xffff, g_ptr_ap_def[i].inst, g_ptr_ap_def[i].val);
		//}
		//分配ini数据临时存储区
		g_ap_diff_size = g_ap_def_inst * 64;//FIXME:应该足够大不会溢出
		debug(2, "alloc mem for ap %d\n", g_ap_diff_size);
		//g_ap_diff_size = INC_SIZE;//FIXME:应该足够大不会溢出
		g_ptr_ap_diff = (unsigned char*)malloc(g_ap_diff_size);
		if(g_ptr_ap_diff == NULL){
			debug(1, "malloc fail\n");
		}
		memset(g_ptr_ap_diff, 0, g_ap_diff_size);

		if(g_ptr_ap_all != NULL){
			free(g_ptr_ap_all);
			g_ptr_ap_all = NULL;
		}
	}

	char str[256] = {};
	struct timeval tv;
	gettimeofday(&tv, NULL);
	struct tm st = {0};
	localtime_r(&tv.tv_sec, &st);

	//进入新的1s时区，保存前1s数据到文件
	if(g_tv_ap.tv_sec != tv.tv_sec){
		if(g_tv_ap.tv_sec != 0){//首次无数据
			struct tm gst = {0};
			localtime_r(&g_tv_ap.tv_sec, &gst);	
			int size = strlen((unsigned char*)g_ptr_ap_diff);
			if(size){
				memset(str, 0, sizeof(str));
				sprintf(str, 
						"log/%d_%04d%02d%02d%02d%02d%02d_ap.ini", 
						vcb_FNr, gst.tm_year+1900, gst.tm_mon+1, 
						gst.tm_mday, gst.tm_hour, gst.tm_min, 
						gst.tm_sec);
				FILE* fp = fopen(str, "wb");
				if(fp == NULL){
					debug(1, "open file %s error\n", str);
					return -1;
				}
				fwrite((unsigned char*)g_ptr_ap_diff, 1, strlen((unsigned char*)g_ptr_ap_diff), fp);
				fclose(fp);
#ifdef LOGBAK
				memset(str, 0, sizeof(str));
				sprintf(str, 
						"log_bak/%d_%04d%02d%02d%02d%02d%02d_ap.ini", 
						vcb_FNr, gst.tm_year+1900, gst.tm_mon+1, 
						gst.tm_mday, gst.tm_hour, gst.tm_min, 
						gst.tm_sec);

				fp = fopen(str, "wb");
				if(fp == NULL){
					debug(1, "open file %s error\n", str);
					return -1;
				}
				fwrite((unsigned char*)g_ptr_ap_diff, 1, strlen((unsigned char*)g_ptr_ap_diff), fp);
				fclose(fp);
#endif
				memset(g_ptr_ap_diff, 0, g_ap_def_inst * sizeof(ap_t));
			}
		}
		g_tv_ap.tv_sec = tv.tv_sec;
		g_tv_ap.tv_usec = tv.tv_usec;
	}

	int diff = (tv.tv_usec/100000 - g_tv_ap.tv_usec/100000)*100;
	pd_t px;
	unsigned short py;
	for(i = 0; i < g_ap_def_inst; i++){
		memset(str, 0, sizeof(str));
		py = 0;
		px.id = g_ptr_ap_def[i].id;
		px.inst = g_ptr_ap_def[i].inst;
		//debug(1, "ap:%ld.%ld, inst:%d\n", (px.id >> 16) & 0xffff, px.id & 0xffff, px.inst);//TEST
		vs_read_process_data(&px, &py);
		if(py != g_ptr_ap_def[i].val){//检查ap值并打印ini记录
			g_ptr_ap_def[i].val = py;
			sprintf(str, "[%d_%ld.%ld_%d %d]\r\n", vcb_FNr, (px.id >> 16) & 0xffff,
					px.id & 0xffff, px.inst, diff);
			sprintf(str+strlen(str), "Value=%d\r\n", py);
			sprintf(str+strlen(str), 
					"Time=%04d%02d%02d%02d%02d%02d%03ld\r\n", 
					st.tm_year+1900, st.tm_mon+1, st.tm_mday, 
					st.tm_hour, st.tm_min, st.tm_sec, 
					(tv.tv_usec/100000)*100);
		}

		if(g_ap_diff_size - strlen(g_ptr_ap_diff) < strlen(str)){//扩展存储区
			debug(1, "===================mem overflow\n");//FIXME:为什么会出错?
			int len = strlen(g_ptr_ap_diff);
			g_ap_diff_size += INC_SIZE;
			g_ptr_ap_diff = (char*)realloc(g_ptr_ap_diff, g_ap_diff_size);
			if(g_ptr_ap_diff == NULL){
				debug(1, "call realloc fail\n");
			}
			memset(g_ptr_ap_diff + len, 0, g_ap_diff_size - len);
		}

		strcat((unsigned char*)g_ptr_ap_diff, str);
	}

	return 0;
}
#endif

int drv_add_message(char* str)
{
	FILE *fp = fopen("message.log", "ab+");
	if(!fp){
		printf("open message fail\n");
		return -1;
	}

	fwrite(str, strlen(str), 1, fp);

	fclose(fp);

	return 0;
}
