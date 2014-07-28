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

	int vcb_sg_exist[] = {0,//sg:1-15
		1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1 };
	//free状态的总时间，包括prep
    int vcb_free_sum[16] = {0, 40, 40, 40, 40, 40, 40, 80, 80, 80, 80, 40, 40, 40, 40, 60};
    //close状态的总，不包括amber
    int vcb_close_sum[16] = {0, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};
    int vcb_prep[16] = {0, 20, 20, 20, 20, 20, 20, 0, 0, 0, 0, 20, 20, 20, 20, 0};
    int vcb_green_blink[16] = {0, 0, 0, 0, 0, 0, 0, 70, 70, 80, 60, 0, 0, 0, 0, 40};
    int vcb_amber[16] = {0, 30, 30, 30, 30, 30, 30, 0, 0, 0, 0, 30, 30, 30, 30, 0};

	//绿间隔
	int vcb_inter_green[16][16] = {
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0, 50},
        { 0,  0,  0, 40,  0, 50, 50, 40,  0, 60,  60, 30, 30,  0,  0,  0},
        { 0,  0,  0, 50, 50,  0, 40, 30, 50,  0,   0, 40, 30,  0,  0,  0},
        { 0, 50, 50,  0, 40, 50,  0, 50, 40,  0,  60,  0,  0,  0,  0,  0},
        { 0,  0, 50, 50,  0,  0, 40, 60, 60, 50,   0, 30, 30,  0,  0,  0},
        { 0, 50,  0, 50,  0,  0, 60,  0,  0, 30,  60, 30, 30,  0,  0,  0},
        { 0, 50, 50,  0, 50, 50,  0, 70, 60, 60,  40, 40, 30,  0,  0,  0},
        { 0,100, 70,100, 90,  0, 90,  0,  0,  0,   0,  0,  0,  0,  0,  0},
        { 0,  0,100,110,100,  0, 90,  0,  0,  0,   0, 70, 70,  0,  0,  0},
        { 0,110,  0,  0,120, 90,110,  0,  0,  0,   0,  0,  0,  0,  0,  0},
        { 0, 80,  0, 70,  0, 70, 90,  0,  0,  0,   0,  0,  0,  0,  0,  0},
        { 0, 90, 70,  0, 80, 90,100,  0, 30,  0,   0,  0,  0,  0,  0,  0},
        { 0, 50, 80,  0, 70, 60, 60,  0, 80,  0,   0,  0,  0,  0,  0,  0},
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0, 50},
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0, 50},
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0, 50, 50,  0}
        };


	int vcb_det_exist[] = {0,//det:1-45
		1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,
		1,1,1,1,1 };


	int vcb_prg_exist[] = {0,//prg:1-6
		1,1,1,1,1,1 };
	int vcb_prg_tu[] = {0,
		720,720,720,720,720,720 };

#endif /*__VCB_H*/
