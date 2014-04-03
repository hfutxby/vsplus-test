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

typedef struct {
	int sum_rising; //上升沿计数
	int sum_falling;
	int state; //占用状态
	int hold; //占用计时
	int free; //总计时
	int fault; //故障
}det_node;

typedef struct{
	int red; //红灯状态
	int red_min; //最小红灯时间设定
	int red_time; //红灯已亮起计时
}sg_node;

#endif /*__SIM_H*/
