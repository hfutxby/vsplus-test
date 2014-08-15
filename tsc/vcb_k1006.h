#ifndef __VCB_H
#define __VCB_H

//vsplus信号灯编号与底板实际信号灯编号转换
int vcb_sg_code[] = {0,
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
};//F15

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

int vcb_sg_exist[] = {0,//1-5,7,13,15,21-24
	1,1,1,1,1,0,1,0,
	0,0,0,0,1,0,1,0,
	0,0,0,0,1,1,1,1
};
//free状态的总时间，包括prep
int vcb_free_sum[] =	{0,	50,	50,	50,	50,	50,	0,	50,	0,	0,	0,	0,	0,	50,	0,	50,	0,	0,	0,	0,	0,	50,	50,	50, 50};
//close状态的总，不包括amber
int vcb_close_sum[] =	{0,	20,	20,	20,	20,	20,	0,	20,	0,	0,	0,	0,	0,	20,	0,	20,	0,	0,	0,	0,	0,	20,	20,	20,	20};
int vcb_prep[] =		{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	10,	10,	10,	10};
int vcb_green_blink[] =	{0,	30,	30,	30,	30,	30,	0,	30,	0,	0,	0,	0,	0,	30,	0,	30,	0,	0,	0,	0,	0,	100,100,100,100};
int vcb_amber[] =		{0,	20,	20,	20,	20,	20,	0,	20,	0,	0,	0,	0,	0,	20,	0,	20,	0,	0,	0,	0,	0,	0,	0,	0,	0};

//绿间隔
int vcb_inter_green[56][3] = {
	{1	,2	,50},
	{1	,4	,50},
	{1	,7	,50},
	{1	,22	,50},
	{1	,24	,50},
	{2	,1	,50},
	{2	,3	,50},
	{2	,5	,50},
	{2	,7	,50},
	{2	,13	,50},
	{2	,21	,50},
	{2	,23	,50},
	{3	,2	,50},
	{3	,4	,50},
	{3	,5	,50},
	{3	,22	,50},
	{3	,24	,50},
	{4	,1	,50},
	{4	,3	,50},
	{4	,5	,50},
	{4	,7	,50},
	{4	,15	,50},
	{4	,21	,50},
	{4	,23	,50},
	{5	,2	,50},
	{5	,3	,50},
	{5	,4	,50},
	{5	,15	,50},
	{5	,22	,50},
	{5	,23	,50},
	{7	,1	,50},
	{7	,2	,50},
	{7	,4	,50},
	{7	,13	,50},
	{7	,21	,50},
	{7	,24	,50},
	{13	,2	,50},
	{13	,7	,50},
	{13	,22	,50},
	{15	,4	,50},
	{15	,5	,50},
	{15	,24	,50},
	{21	,2	,120},
	{21	,4	,120},
	{21	,7	,120},
	{22	,1	,120},
	{22	,3	,120},
	{22	,5	,120},
	{22	,13	,120},
	{23	,2	,120},
	{23	,4	,120},
	{23	,5	,120},
	{24	,1	,120},
	{24	,3	,120},
	{24	,7	,120},
	{24	,15	,120},
};


int vcb_det_exist[] = {0,//det:1-16,18-25,49-52
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	0,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	1,1,1,1
};

int vcb_prg_exist[] = {0,//prg:1-4
	1,1,1,1 
};
int vcb_prg_tu[] = {0,
	900,720,800,900
};

int vcb_ZNr = 1;
int vcb_FNr = 1006;
int vcb_Realknoten = 0;

#endif /*__VCB_H*/
