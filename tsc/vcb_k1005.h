#ifndef __VCB_H
#define __VCB_H

//vsplus信号灯编号与底板实际信号灯编号转换
int vcb_sg_code[] = {0,
        0x41,//D1
        0x11,//D2
        0x22,//D3
        0x43,//D4
        0x23,//D5
        0x24,//D6
        0x62,0x63,0x64,0x61,//F7-F10
        0,0,//FL11-FL12
        //0,0,0,0,0,0,//DR1-6
        0,//D13
        0,//D14
        0};//F15

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

int vcb_sg_exist[] = {0,//2-4,6,16,21,23,24
	0,1,1,1,0,1,0,0,
	0,0,0,0,0,0,0,1,
	0,0,0,0,1,0,1,1
};
//free状态的总时间，包括prep
int vcb_free_sum[] =	{0,	0,	20,	20,	20,	0,	20,	0,	0,	0,	0,	0,	0,	0,	0,	0,	20,	0,	0,	0,	0,	20,	0,	20,	20};
//close状态的总，不包括amber
int vcb_close_sum[] =	{0,	0,	20,	20,	20,	0,	20,	0,	0,	0,	0,	0,	0,	0,	0,	0,	20,	0,	0,	0,	0,	20,	0,	20,	20};
int vcb_prep[] =		{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0};
int vcb_green_blink[] =	{0,	0,	30,	30,	30,	0,	30,	0,	0,	0,	0,	0,	0,	0,	0,	0,	30,	0,	0,	0,	0,	100,0,100,100};
int vcb_amber[] =		{0,	0,	20,	20,	20,	0,	20,	0,	0,	0,	0,	0,	0,	0,	0,	0,	20,	0,	0,	0,	0,	0,	0,	0,	0};

//绿间隔
int vcb_inter_green[22][3] = {
	{2,	3,	50},
	{2,	23,	50},
	{2,	21,	50},
	{3,	4,	50},
	{3,	2,	50},
	{3,	6,	50},
	{3,	24,	50},
	{4,	6,	50},
	{4,	3,	50},
	{4,	23,	50},
	{4,	21,	50},
	{6,	4,	50},
	{6,	3,	50},
	{6,	23,	50},
	{6,	21,	50},
	{21,4,	100},
	{21,16,	100},
	{21,2,	100},
	{23,4,	100},
	{23,2,	100},
	{23,6,	100},
	{24,3,	100}
};


	int vcb_det_exist[] = {0,//det:1-13
		1,1,1,1,1,1,1,1,
		1,1,1,1,1
	};

	int vcb_prg_exist[] = {0,//prg:1
		1 
	};
	int vcb_prg_tu[] = {0,
		900
	};

	int vcb_ZNr = 1;
	int vcb_FNr = 1005;
	int vcb_Realknoten = 0;

#endif /*__VCB_H*/
