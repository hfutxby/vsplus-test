#ifndef _SG_TRACK_H
#define _SG_TRACK_H

//信号灯记录信息
typedef struct{
    int stat; //信号灯状态，0-disable;1-amber;2-min_red;3-ex_red;4-prep;5-min_green;6-ex_green
    int time; //信号灯状态计时
}sg_track;

#endif /*_SG_TRACK_H*/
