#ifndef _SG_TRACK_H
#define _SG_TRACK_H

//信号灯记录信息
typedef struct{
    int stat; //信号灯状态，0-disable;1-amber;2-min_red;3-ex_red;4-prep;5-min_green;6-ex_green
    int time; //信号灯状态计时
	int fault;//1：故障，0：正常
}sg_track;

int init_sg_track(void);
void deinit_sg_track(void);
int sg_track_fault(int sg);
int sg_track_fault_set(int sg, int type);
int sg_track_chk(int sg, int stat);
void sg_track_switch(int sg, int stat);

#endif /*_SG_TRACK_H*/
