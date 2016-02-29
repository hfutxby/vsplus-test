#ifndef __TEST_MSG_H
#define __TEST_MSG_H

//#include "if626max.h"

// =========== cmd  head =============
//set detector slopes and faults
#define	SET_DET	1
//get all aviliable detectors
#define	GET_DETS		2
//reponse to GET_DETS
#define GET_DETS_R	3
//set a pt message
#define SET_PT	4
//switch a program
#define SET_PRG		5
//sg switch
#define	SG_SWITCH	6
//set vsplus running stat
#define	SET_VSPLUS_STAT	7

#define TEST 99

// =========== cmd body =============
struct test_t {
	int time;
};

struct sg_switch_t{
	char sg;
	char stat;
};

struct set_vsplus_stat_t{
	char stat;//0-stop, 1-start
};
//enum msg_type {
//SET_DET = 1, //设置检测器状态
//DET_EXIST,//请求获得检测器配置
//DET_EXIST_R,
//SET_PT,//发送一个PT消息
//SET_PRG,
//SET_STAT,//change vsplus run stat
//SG,
//TEST
//};

struct cmd_msg_head_t {
	unsigned short tag; //0xF1, 0xF2
	unsigned short  type;//begin with b1111
	unsigned short len;//begin with b1010, data size
//	void* data;
};

//det
//enum det_stat {
//	RISING = 1, FALLING, FAULT, OK
//};

//struct set_det_data {
//	int id;
//	enum det_stat stat;
//};

//struct det_exist_data {
//	char exist[340]; //FIXME
//};

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
struct set_pt_data {
	short head;
	short MP; // (call point);
	short Linie; // (line);
	short Kurs; // (course);
	short Route; // (route);
	short Prioritaet; // (priority);
	short Laenge; // (vehicle length);
	short RichtungVonHand; // (direction by hand);
	short FahrplanAbweichnung; // (difference to schedule);
};

#endif /*__TEST_MSG_H*/
