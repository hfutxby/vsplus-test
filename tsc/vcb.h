#ifndef __VCB_H
#define __VCB_H

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

//vsplus信号灯编号与底板实际信号灯编号转换
int vcb_sg_code[MAX_SIG_GROUP+1] = {0,
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
int vcb_sg_stat_code[] = {
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

int vcb_sg_exist[MAX_SIG_GROUP+1] = {0};

int vcb_free_sum[MAX_SIG_GROUP+1] = {0};
int vcb_close_sum[MAX_SIG_GROUP+1] = {0};
int vcb_prep[MAX_SIG_GROUP+1] = {0};
int vcb_amber[MAX_SIG_GROUP+1] = {0};
int vcb_green_blink[MAX_SIG_GROUP+1] = {0};

//绿间隔
int vcb_inter_green[MAX_SIG_GROUP][MAX_SIG_GROUP] = {0};

int vcb_det_exist[MAX_DET_GROUP+1] = {0};

int vcb_prg_exist[MAX_VSP_PROG] = {0};
int vcb_prg_tu[MAX_VSP_PROG] = {0};

int vcb_ZNr = 0;
int vcb_FNr = 0;
int vcb_Realknoten = 0;

#endif /*__VCB_H*/
