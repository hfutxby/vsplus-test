#ifndef _PRG_TRACK_
#define _PRG_TRACK_

//#define PRGMAX 10 //方案数
//设置下一个配时方案
int prg_track_next_set(int index);

typedef struct{
	int tx; //方案内计时
	int stat;//1：当前方案，2：下一个方案，0：其它
}prg_track;

#endif /*_PRG_TRACK_*/
