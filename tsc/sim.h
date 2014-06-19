#ifndef __SIM_H
#define __SIM_H
int sim_init(void);
int sim_deinit(void);
int sim_prog_actual(void);
int sim_prog_select(void);

typedef struct{
	int index;
	int cycle_time;
	int cur_time;
}prog;

//一般模拟控制参数
typedef struct{
    int prg_cur;//当前运行的配时方案
    int prg_next;//等待切换运行的配时方案
}ctrl_data;

////检测器模拟参数
//typedef struct {
//	int sum_rising; //上升沿计数
//	int sum_falling; //下降沿计数
//	int state; //计时状态，<100:占用中，>100:空闲中
//	int hold; //总占用计时
//	int free; //总空闲计时
//	int fault; //故障
//	int occ1;//占用率
//	int occ2;//平滑占用率
//	int net;//net time gap starts at the last falling slope
//	int gross;//gross time gap starts at the last rising slope
//}det_node;

//信号灯记录信息
//typedef struct{
//	int stat; //信号灯状态，0-disable;1-amber;2-min_red;3-ex_red;4-prep;5-min_green;6-ex_green
//	int red_min; //最小红灯时间设定
//	int prep; //红绿过渡时间设定
//	int green_min; //最小绿灯时间设定
//	int amber;//绿红过渡时间设定
//	int time; //信号灯状态计时
//	int ext;//b00状态未变，b01进行红绿切换，b10进行绿红切换
//}sg_node;

#endif /*__SIM_H*/
