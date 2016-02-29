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
  short sw_off_time;
  short sw_on_time;
  char sw_off_sg[MAX_SIG_GROUP];
  char sw_on_sg[MAX_SIG_GROUP];
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


