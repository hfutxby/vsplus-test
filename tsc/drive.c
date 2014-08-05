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
#include "parse_xml.h"
#include "vcb.h"
#include "if626bas.h"

/*====== 信号灯操作 =======*/
//取得信号灯编号的代码
int drv_sg_code(int sg)
{
#if 0
	int xml_sg_code(int sg);
#else
	if(sg < (sizeof(vcb_sg_code) / sizeof(int)))
		return vcb_sg_code[sg];
	else
		return 0;
#endif
}

//取得信号灯状态的代码
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
	//printf("==========drv_sg_switch(%d, %d)===========\n", sg, stat);
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

#if 0
//检查信号灯的状态
//如果sg处于stat状态，返回此状态时间，否则返回-1
int drv_sg_chk(int sg, int stat)
{
	int ret;
	ret = sg_track_chk(sg, stat);

	return ret;
}
#endif

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
		sg->min_green = vcb_free_sum[i] - vcb_prep[i];//green_blink作为close
		sg->prep = vcb_prep[i];
		sg->amber = vcb_amber[i];
		sg->green_blink = vcb_green_blink[i];
		sg->ext = 0;
		sg->exist = vcb_sg_exist[i];
	}

	return 0;
#endif
}

void drv_sg_para_dump(void* ptr, int size)
{
	int i;
	int num = size / sizeof(sg_def);
	sg_def* sg = NULL;
	printf("===%s===\n", __func__);
	//printf("sg_id\t min_red\t min_green\t prep\n");
	printf("%9s %9s %9s %9s\n", "sg_id", "min_red", "min_green", "prep");
	for(i = 0; i < num; i++){
		sg = (sg_def*)(ptr + i * sizeof(sg_def));
		if(sg->exist)
			printf("%9d %9d %9d %9d\n", i, sg->min_red, sg->min_green, sg->prep);
	}
}

#if 0
//检查信号灯是否故障
int drv_sg_fault(int index)
{
	return sg_track_fault(index);
}
#endif

/*===== 检测器操作 ======*/
#if 0
void drv_det_op(int index, int op)
{
	tsc_det_op(index, op);
}
#endif

//获取检测器配置参数
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

/*===== 配时方案 ====*/
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

int drv_prg_next_set(int index)
{
	return prg_track_next_set(index);
}

int drv_inter_green(int sgr, int sge)
{
	int num = sizeof(vcb_inter_green) / (sizeof(int)*3);
	int i;
	for(i = 0; i < num; i++){
		if((vcb_inter_green[i][0] == sgr) && (vcb_inter_green[i][1] == sge)){
			return vcb_inter_green[i][2];
		}
	}

	return 0;
	//return 32000;
}

int drv_get_ocitid(int* ZNr, int* FNr, int*Realknoten)
{
	*ZNr = vcb_ZNr;
	*FNr = vcb_FNr;
	*Realknoten = vcb_Realknoten;

	return 1;
}

static char *ptr_det = NULL;
static int g_size_det = 0;
static int g_space_det = 0;
static struct timeval g_tv_det = {};
static int g_dir_det = 1;
//write ini file
int drv_add_det(int det, int value)
{
	//首次确认目录存在
	if(g_dir_det){
		if(access("log", F_OK) == -1){
			if(mkdir("log", 0777) == -1)
				perror("mkdir log");
		}
		if(access("log/det", F_OK) == -1){
			if(mkdir("log/det", 0777) == -1)
				perror("mkdir log/det");
		}
		g_dir_det = 0;
		g_size_det = 1024;
		g_space_det = g_size_det;
		ptr_det = malloc(g_size_det);//首次分配空间,FIXME:泄露风险
		memset(ptr_det, 0, g_size_det);
	}

	char str[256] = {};
	struct timeval tv;
	gettimeofday(&tv, NULL);
	time_t tt = tv.tv_sec;
    struct tm *t = localtime(&tt);

	if(g_tv_det.tv_sec != tv.tv_sec){
		if(g_tv_det.tv_sec != 0){//保存文件
			memset(str, 0, sizeof(str));
			sprintf(str, "log/det/%d_%04d%02d%02d%02d%02d%02d.ini", vcb_FNr, t->tm_year+1900, 
					t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
			FILE* fp = fopen(str, "wb");
			if(fp == NULL){
				debug(1, "open file %s error\n", str);
				return -1;
			}
			fwrite(ptr_det, g_size_det - g_space_det, 1, fp);
			fclose(fp);
			memset(ptr_det, 0, g_size_det);
			g_space_det = g_size_det;
		}
		g_tv_det.tv_sec = tv.tv_sec;
		g_tv_det.tv_usec = tv.tv_usec;
	}

	memset(str, 0, sizeof(str));
	int diff = (tv.tv_usec/1000 - g_tv_det.tv_usec/1000);
	sprintf(str, "[%d_D_%d %d]\n", vcb_FNr, det, diff);
	sprintf(str+strlen(str), "Value=%d\n", value);
	sprintf(str+strlen(str), "Time=%04d%02d%02d%02d%02d%02d%03ld\n\n", t->tm_year+1900, 
        t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec/1000);

	if(g_space_det < strlen(str)){//扩展存储区
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

char *ptr_sg = NULL;
static int g_size_sg = 0;
static int g_space_sg = 0;
static struct timeval g_tv_sg = {};
static int g_dir_sg = 1;
//write ini file
int drv_add_sg(int sg, int stat)
{
	//首次确认目录存在
	if(g_dir_sg){
		if(access("log", F_OK) == -1){
			if(mkdir("log", 0777) == -1)
				perror("mkdir log");
		}
		if(access("log/sg", F_OK) == -1){
			if(mkdir("log/sg", 0777) == -1)
				perror("mkdir log/sg");
		}
		g_dir_sg = 0;
		g_size_sg = 1024;
		g_space_sg = g_size_sg;
		ptr_sg = malloc(g_size_sg);//首次分配空间,FIXME:泄露风险
		memset(ptr_sg, 0, g_size_sg);
	}

	//ext read, ext green实际没有产生灯色变化
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
			value = 0b00000011;//2
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
	time_t tt = tv.tv_sec;
    struct tm *t = localtime(&tt);

	if(g_tv_sg.tv_sec != tv.tv_sec){
		if(g_tv_sg.tv_sec != 0){//保存文件
			memset(str, 0, sizeof(str));
			sprintf(str, "log/sg/%d_%04d%02d%02d%02d%02d%02d.ini", vcb_FNr, t->tm_year+1900, 
					t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
			FILE* fp = fopen(str, "wb");
			if(fp == NULL){
				debug(1, "open file %s error\n", str);
				return -1;
			}
			fwrite(ptr_sg, g_size_sg - g_space_sg, 1, fp);
			fclose(fp);
			memset(ptr_sg, 0, g_size_sg);
			g_space_sg = g_size_sg;
		}
		g_tv_sg.tv_sec = tv.tv_sec;
		g_tv_sg.tv_usec = tv.tv_usec;
	}

	memset(str, 0, sizeof(str));
	int diff = (tv.tv_usec/1000 - g_tv_sg.tv_usec/1000);
	sprintf(str, "[%d_S_%d %d]\n", vcb_FNr, sg, diff);
	sprintf(str+strlen(str), "Value=%d\n", value);
	sprintf(str+strlen(str), "Time=%04d%02d%02d%02d%02d%02d%03ld\n\n", t->tm_year+1900, 
        t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec/1000);

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
	strcat(ptr_sg, str);
	g_space_sg -= strlen(str);
	return 0;
}

typedef struct {
    long id;
    unsigned short inst;
    unsigned char KmpInd;
    unsigned char ErgLaenge;
}pd_t;

#if 1


//打印所有ap
static int g_dir_ap = 1; //首次调用标志
static pd_t *ptr_pd = NULL;//所有可供调用ap
static int ptr_pd_size = 0;//所有可供调用ap数量
static int g_ap_def[120] = {};//实际需要的ap
static int g_ap_def_num = 0;//实际需要的ap数量
static int g_ap_def_inst = 0;//实际需要的inst数量
static char* ptr_ap = NULL;//写入文件前的数据
static int g_size_ap = 0;
static int g_space_ap = 0;

static struct timeval g_tv_ap = {};

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

int getinst(char* line)
{
	int i;
	int id = getid(line);
	int num = ptr_pd_size / sizeof(pd_t);
	for(i = 0; i < num; i++){
		if(ptr_pd[i].id == id)
			return ptr_pd[i].inst;
	}

	return 0;
}

int getinst2(int id)
{
	int i;
	int num = ptr_pd_size / sizeof(pd_t);
	for(i = 0; i < num; i++){
		if(ptr_pd[i].id == id)
			return ptr_pd[i].inst;
	}

	return 0;
}

int drv_add_ap(void)
{
	//确认目录存在
	if(g_dir_ap){
		g_dir_ap = 0;
		if(access("log", F_OK) == -1){
			mkdir("log", 0777);
		}
		if(access("log/ap", F_OK) == -1){
			mkdir("log/ap", 0777);
		}
		//所有存在的ap
		FILE* fp = fopen("ap.dat", "wb");
		int ret = vs_read_process_data(NULL, fp);
		ptr_pd_size = ftell(fp);
		fclose(fp);

		ptr_pd = malloc(ptr_pd_size);
		memset(ptr_pd, 0, ptr_pd_size);
		fp = fopen("ap.dat", "rb");
		fread(ptr_pd, ptr_pd_size, 1, fp);
		fclose(fp);

		//需要打印的ap
		fp = fopen("ap.def", "rb");
		if(fp == NULL){
			printf("open file ap.def error\n");
			g_ap_def_num = 1;//默认
			g_ap_def[0] = (57 << 16) | 0;
		}
		char *line = NULL;
		int size, n, i = 0;
		while((size = getline(&line, &n, fp)) != -1){
			g_ap_def[i] = getid(line);
			g_ap_def_num++;
			g_ap_def_inst += getinst(line);
			i++;
		}
		fclose(fp);
		g_size_ap = g_ap_def_inst * 64;
		g_space_ap = g_size_ap;
		ptr_ap = malloc(g_size_ap);
		memset(ptr_ap, 0, g_size_ap);
	}


	char str[256] = {};
	struct timeval tv;
	gettimeofday(&tv, NULL);
	time_t tt = tv.tv_sec;
	struct tm *t = localtime(&tt);

	if(g_tv_ap.tv_sec != tv.tv_sec){
		if(g_tv_ap.tv_sec != 0){//保存文件
			memset(str, 0, sizeof(str));
			sprintf(str, "log/ap/%d_%04d%02d%02d%02d%02d%02d.ini", vcb_FNr, t->tm_year+1900, 
					t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
			FILE* fp = fopen(str, "wb");
			if(fp == NULL){
				debug(1, "open file %s error\n", str);
				return -1;
			}
			fwrite(ptr_ap, g_size_ap - g_space_ap, 1, fp);
			fclose(fp);
			memset(ptr_ap, 0, g_size_ap);
			g_space_ap = g_size_ap;
		}
		g_tv_ap.tv_sec = tv.tv_sec;
		g_tv_ap.tv_usec = tv.tv_usec;
	}

	int diff = (tv.tv_usec/1000 - g_tv_ap.tv_usec/1000);
	int i, j;
	pd_t px;
	unsigned short py;
	for(i = 0; i < g_ap_def_num; i++){
		int inst = getinst2(g_ap_def[i]);
		for(j = 1; j <= inst; j++){
			memset(str, 0, sizeof(str));
			px.id = g_ap_def[i];
			px.inst = j;
			//memset(py, 0, sizeof(py));
			vs_read_process_data(&px, &py);
			//if(ret == 0)
			//    printf("%-3d ", *(unsigned short*)py);
			//else
			//    printf("#   ");//invalid value
			sprintf(str, "[%d_%d.%d_%d %d]\n", vcb_FNr, g_ap_def[i] >> 16 & 0xffff, 
					g_ap_def[i] & 0xffff, j, diff);
			sprintf(str+strlen(str), "Value=%d\n", py);
			sprintf(str+strlen(str), "Time=%04d%02d%02d%02d%02d%02d%03ld\n\n", t->tm_year+1900, 
					t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec/1000);

			if(g_space_ap < strlen(str)){//扩展存储区
				char* ptr = malloc(g_size_ap+1024);
				memset(ptr, 0, g_size_ap+1024);
				memcpy(ptr, ptr_ap, strlen(ptr_ap));
				free(ptr_ap);
				ptr_ap = ptr;
				ptr = NULL;
				g_size_ap += 1024;
				g_space_ap += 1024;
			}	
			strcat(ptr_ap, str);
			g_space_ap -= strlen(str);
		}
	}
	return 0;
}
#endif


#if 0
//按需打印ap
static int g_dir_ap = 1;
static struct timeval g_tv_ap = {};
static int g_num_ap = 0;
static pd_t *ptr_ap = NULL;
int drv_add_ap(void)
{
//	FILE* fp = fopen("ap.dat", "wb");
//	int ret = vs_read_process_data(NULL, fp);
//	fclose(fp);
//	return 0;

	//确认目录存在
	if(g_dir_ap){
		g_dir_ap = 0;
		if(access("log", F_OK) == -1){
			mkdir("log", 0777);
		}
		if(access("log/ap", F_OK) == -1){
			mkdir("log/ap", 0777);
		}
		FILE *fp = fopen("ap.def", "rb");
		if(fp == NULL){
			printf("open file ap.def error\n");
			g_num_ap = 1;//默认
			ptr = malloc(sizeof(pd_t));
			memset(ptr, 0, sizeof(pd_t));
			ptr[0].id = 57 << 16 | 0;
		}
		char *line = NULL;
		int size, n;
		int a, b, i;
		while((size = getline(&line, &n, fp)) != -1){
			g_num_ap++;
		}
		ptr_ap = malloc(sizeof(pd_t) * g_num_ap);
		memset(ptr_ap, 0, sizeof(pd_t) * g_num_ap);
		debug(2, "=======================g_num_ap:%d\n", g_num_ap);
		fseek(fp, 0, SEEK_SET);
		while((size = getline(&line, &n, fp)) != -1){
			a = atoi(line);
			for(i = 0; i < size; i++){
				if(line[i] == '.')
					break;
			}
			if(i != (size-1))
				b = atoi(line+i+1);
			else
				b = 0;
		}
	}

	char str[256] = {};
	struct timeval tv;
	gettimeofday(&tv, NULL);
	time_t tt = tv.tv_sec;
	struct tm *t = localtime(&tt);

	sprintf(str, "log/ap/%d_%04d%02d%02d%02d%02d%02d.ini", vcb_FNr, t->tm_year+1900, 
			t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	if(access(str, F_OK) == -1){
		debug(2, "first create file %s\n", str);
		g_tv_sg = tv;
	}
	FILE* fp = fopen(str, "a+");
	if(fp == NULL){

		debug(1, "open file %s error\n", str);
	}
	int diff = (tv.tv_usec/1000- g_tv_sg.tv_usec/1000);
	fprintf(fp, "[%d_S_%d %d]\n", vcb_FNr, sg, diff);
	fprintf(fp, "Value=%d\n", value);
	fprintf(fp, "Time=%04d%02d%02d%02d%02d%02d%03ld\n\n", t->tm_year+1900, 
			t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec/1000);

	fclose(fp);

#if 0
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
#endif
}
#endif
