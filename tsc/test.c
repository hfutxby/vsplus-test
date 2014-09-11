#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

#include "if626bas.h"
#include "if626max.h"
#include "tsc.h"
#include "vcb.h"

#define VSP_PARAMFILE "./vsp_param.dat"
#define MAX_VSP_PROG 16
#define MAX_DET_GROUP 128
#define MAX_SIG_GROUP 64

typedef struct VSPSysData_t
{
  int area_num;  //区域编号
  int con_num;   //信号机控制器编号
  int node_num;  //节点编号
  short active_prog;  //当前活动方案号
  short vsp_cycletime[MAX_VSP_PROG];  //各方案周期值
  short inter_green[MAX_SIG_GROUP][MAX_SIG_GROUP];  //间隔绿
} VSPSysData_t;

typedef struct VSPDetDataList_t
{
  char det_valid;  //检测器标志(0:无效，1:有效)
  int smooth_occupdegree;  //平滑占用率
} VSPDetDataList_t;

typedef struct VSPDetData_t
{
  unsigned char MaxDetDataNum;
  VSPDetDataList_t VSPDetDataList[MAX_DET_GROUP];
} VSPDetData_t;

typedef struct VSPSigDataList_t
{
  char sig_valid;  //信号灯组标志(0:无效，1:有效)
  char sig_state;  //信号灯组状态(7:绿闪，1:黄，2:最小红，3:扩展红，4:红黄，5:最小绿，6:扩展绿)
  short sig_count;  //信号灯组状态时间计数值，初始为0
  unsigned int red_sec;  //信号灯组红灯开始时间，初始为0
  unsigned int green_sec;  //信号灯组绿灯开始时间，初始为0
  short amber_time;  //amber时间
  short minred_time;  //minred时间
  short prep_time;    //prep时间
  short mingreen_time;  //mingreen时间
  short green_blink;  //greenblink时间
  char first_flag;  //信号灯组首次运行标志(0:未运行，1:已运行)，初始为0
} VSPSigDataList_t;

typedef struct VSPSigData_t
{
  unsigned char MaxSigDataNum;
  VSPSigDataList_t VSPSigDataList[MAX_SIG_GROUP];
} VSPSigData_t;

VSPSysData_t VSPSysData;
VSPDetData_t VSPDetData;
VSPSigData_t VSPSigData;

void dump_vspconfig(void)
{
    int i, j;
    printf("=== VSPSigData ===\n");
    printf("%6s %6s %6s %6s %6s %6s %6s\n", "id", "state", "amber", "red", "prep", "green", "blink");
    for(i = 0; i < VSPSigData.MaxSigDataNum; i++){
        if(VSPSigData.VSPSigDataList[i].sig_valid){
            printf("%6d %6d %6d %6d %6d %6d %6d \n", i+1, VSPSigData.VSPSigDataList[i].sig_state,
                    VSPSigData.VSPSigDataList[i].amber_time,
                    VSPSigData.VSPSigDataList[i].minred_time,
                    VSPSigData.VSPSigDataList[i].prep_time,
                    VSPSigData.VSPSigDataList[i].mingreen_time,
                    VSPSigData.VSPSigDataList[i].green_blink);
        }
        else
            printf("%6d %6s %6s %6s %6s %6s %6s\n", i+1, "invalid", "-", "-", "-", "-", "-");
    }

    printf("=== VSPDetData ===\n");
    printf("%6s %6s\n", "id", "exist");
    for(i = 0; i < VSPDetData.MaxDetDataNum; i++){
        if(VSPDetData.VSPDetDataList[i].det_valid)
            printf("%6d %6d\n", i+1, 1);
        else
            printf("%6d %6s\n", i+1, "invalid");
    }

    printf("=== VSPSysData ===\n");
    printf("ZNr:%d, FNr:%d, Relknoten:%d\n", VSPSysData.area_num,
            VSPSysData.con_num, VSPSysData.node_num);
    printf("active_prog:%d\n", VSPSysData.active_prog);
    for(i = 0; i < MAX_VSP_PROG; i++){
        if(VSPSysData.vsp_cycletime[i])
            printf("prg_id:%d tu:%6d\n", i+1, VSPSysData.vsp_cycletime[i]);
    }

    printf("%4s %4s %4s\n", "sgr", "sge", "val");
    for(i = 0; i < MAX_SIG_GROUP; i++){
        for(j = 0; j < MAX_SIG_GROUP; j++){
            if(VSPSysData.inter_green[i][j])
                printf("%4d %4d %4d\n", i+1, j+1, VSPSysData.inter_green[i][j]);
        }
    }
}

int drv_read_vsp_para(char* filename)
{
	int i, j, len;
	int ret = 0;

	//read
	FILE* fp = fopen(filename, "rb");
	if(fp == NULL){
		debug(1, "fopen %s error\n", filename);
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
	dump_vspconfig();

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
		vcb_free_sum[i+1] = VSPSigData.VSPSigDataList[i].mingreen_time + VSPSigData.VSPSigDataList[i].prep_time;
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
#if 0
	printf("ZNr:%d\nFNr:%d\nRealknoten:%d\n", vcb_ZNr, vcb_FNr, vcb_Realknoten);
#endif

	return ret;
}

int main(void)
{

	drv_read_vsp_para(VSP_PARAMFILE);

	return 0;
}
