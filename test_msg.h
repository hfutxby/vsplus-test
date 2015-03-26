#ifndef __TEST_MSG_H
#define __TEST_MSG_H

#include "if626max.h"

enum msg_type{
	SET_DET = 1,//设置检测器状态
	DET_EXIST,//请求获得检测器配置
	DET_EXIST_R,
	SET_PT,//发送一个PT消息
	SET_PRG,
	SG,
	TEST
};

struct msg_head{
	enum msg_type type;
	short len;
};

//det
enum det_stat{
	RISING = 1,
	FALLING,
	FAULT,
	OK
};

struct set_det_data{
	int id;
	enum det_stat stat;
};

struct det_exist_data{
	char exist[DETMAX];
};

////PT
//struct set_pt_data{
//	int MP;// (call point);
//	int Linie;// (line);
//	int Kurs;// (course);
//	int Route;// (route);
//	int Prioritaet;// (priority);
//	int Laenge;// (vehicle length);
//	int RichtungVonHand;// (direction by hand);
//	int FahrplanAbweichnung;// (difference to schedule);
//};
//PT
struct set_pt_data{
	short head;
	short MP;// (call point);
	short Linie;// (line);
	short Kurs;// (course);
	short Route;// (route);
	short Prioritaet;// (priority);
	short Laenge;// (vehicle length);
	short RichtungVonHand;// (direction by hand);
	short FahrplanAbweichnung;// (difference to schedule);
};

#endif /*__TEST_MSG_H*/
