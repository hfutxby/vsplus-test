#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#include "tsc.h"
#include "sim.h"
#include "if626bas.h"
#include "if626max.h"

void* g_mem[3]; //VSPLUS内部分配用于存储参数
FILE* g_vcb_file = NULL; //配置参数vcb文件
FILE* g_vcb_back = NULL; //备份参数vcb文件
#define VCB_FILE "para.vcb" //配置参数vcb文件
#define VCB_BACK "para_bak.vcb" //备份参数vcb文件
#define _SIM_TEST_ //使用模拟数据进行测试
#define _TEST_SG_ //使用串口指令进行信号切换

int g_exit;//退出线程标志

int g_timer[MAXTIMER];//最低位做开关标志
pthread_t g_tid_timer;//定时器计时线程

int g_fd_serial;//和驱动版通信串口
pthread_t g_tid_watchdog; //喂狗线程

int g_fd_sg; //记录信号灯状态的文件
sg_node* g_sg; //信号灯参数
pthread_t g_tid_sg;//记录信号灯状态的线程

xml_para* g_xml_para; //从xml解析出来的配置参数

/****************** 参数配置相关函数 **************************/
/* 分配存储区  */
void* tsc_alloc_mem(int size, int id)
{
	if((id > 3) || (id < 1)){
		debug(1, "id range (1-3)\n");
		return NULL;
	}
	else{
		if(g_mem[id-1] != NULL ){
			debug(2, "area %d already allocated\n", id);
			return g_mem[id-1];
		}
		else{
			g_mem[id-1] = malloc(size);
			if(g_mem[id-1] == NULL){
				debug(1, "area %d allocate fail\n", id);
				return NULL;
			}
			return g_mem[id-1];
		}
	}
}

/* 释放存储区 */
void tsc_free_mem(int id)
{
	if((id > 3) || (id < 1)){
		debug(1, "id range (1-3)\n");
	}
	else{
		if(g_mem[id-1] == NULL){
			debug(2, "area %d already freed\n", id);
		}
		free(g_mem[id-1]);
		g_mem[id-1] = NULL;
	}
}

/* 取存储区地址 */
void* tsc_get_mem(int id)
{
	if((id > 3) || (id < 1)){
		debug(1, "id range (1-3)\n");
		return NULL;
	}
	else{
		if(g_mem[id-1] == NULL){
			debug(2, "area %d not allocated\n", id);
			return NULL;
		}
		//debug(3, "area %d:\"%s\"\n", id, (char*)g_mem[id-1]);
		return g_mem[id-1];
	}
}

/* 打开配置参数VCB文件 */
int tsc_open_vcb(void)
{
	if(g_vcb_file != NULL){
		debug(2, "vcb file already opened\n");
		return 1;
	}
	else{
		g_vcb_file = fopen(VCB_FILE, "rb");
		if(g_vcb_file == NULL){
			debug(1, "%s\n", strerror(errno));
			return 0;
		}
		return 1;
	}
}

/* 关闭配置参数VCB文件 */
void tsc_close_vcb(void)
{
	if(g_vcb_file == NULL){
		debug(2, "vcb file not opened\n");
		return;
	}
	else{
		if(fclose(g_vcb_file) != 0){
			debug(1, "%s\n", strerror(errno));
		}
		g_vcb_file = NULL;
	}
}

/* 读配置参数VCB数据 */
int tsc_read_vcb(char *data, int size)
{
	if(g_vcb_file == NULL){
		debug(1, "vcb file not opened\n");
		return -1;
	}
	memset(data, 0, size);
	return fread(data, 1, size, g_vcb_file);

}

/* 打开备份vcb文件 */
int tsc_open_back(void)
{
    g_vcb_back = fopen(VCB_BACK, "wb+");
    if(!g_vcb_back){
        debug(1, "open backup vcb file error\n");
        return 0;
    }

    return 1;
}

/* 写入备份vcb文件 */
int tsc_write_back(char* data, int size)
{
    if(!g_vcb_back){
        debug(1, "backup vcb file not opened yet!\n");
        return;
    }
    return fwrite(data, 1, size, g_vcb_back);
}

/* 关闭备份文件 */
void tsc_close_back(void)
{
    if(!g_vcb_back){
        debug(2, "backup vcb file not opened yet!\n");
    }
    fclose(g_vcb_back);
    g_vcb_back = 0;
}

/****************** 定时器 ***************************/
void time_go(void)
{
	//debug(3, "==>\n");
	int i;
	for(i = 0; i < MAXTIMER; i++){
		if(g_timer[i] >= MAXCOUNT){//达到最大值
			g_timer[i] = MAXCOUNT;
		}
		else if(g_timer[i] % 2){
			g_timer[i] += 2;
		}
	}
}

int us_sleep(long us)
{
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = us;
	return select(0, NULL, NULL, NULL, &tv);
}

int thr_timer(void* arg)
{
	struct timeval tv1, tv2;
	int us = 100000, diff;
	while(!g_exit){
		gettimeofday(&tv1, NULL);
		time_go();
		gettimeofday(&tv2, NULL);
		diff = (tv2.tv_sec-tv1.tv_sec)*1000000+(tv2.tv_usec-tv1.tv_usec);
		//printf("time_go:%dus\n", diff);
		us_sleep(us-diff);
	}
}

/* 定时器 */
int tsc_timer(int func, int index, int count)
{
	//debug(3, "==>\n");
	int ret = 0;
	int i = 0;
	if((index >= 0) && (index <= MAXTIMER)){
		switch(func){
			case 1://read
				ret = g_timer[index]>>1;
				break;
			case 2://load and start
				g_timer[index] = (count<<1) + 1;
				break;
			case 3://clear
				g_timer[index] = 1;
				break;
			case 4://stop and clear
				g_timer[index] = 0;
				break;
			case 5://stop
				g_timer[index] &= 0xfffffffe;
				break;
			case 6://test
				debug(2, "test\n");
				for(i = 0; i < MAXTIMER; i++){
					printf("timer[%d],%s,count=%d\n", i, (g_timer[i]%2) ? "running" : "stopped", g_timer[i]>>1);
				}
				break;
		}
	}

	return ret;
}

int init_timers(void)
{
	debug(3, "==>\n");
	int i, ret;
	for(i = 0; i < MAXTIMER; i++)//初始化清零所有计数
		g_timer[i] = 0;

	ret = pthread_create(&g_tid_timer, NULL, thr_timer, NULL);
	if(ret != 0){
		printf("%s(%d):pthread_create error: %s\n", __func__, __LINE__, strerror(errno));
		return -1;
	}

	debug(3, "<==\n");

	return 0;
}

int deinit_timers(void)
{
	debug(3, "==>\n");
	g_exit = 1;
	
	int ret;
	ret = pthread_join(g_tid_timer, NULL);
	if(ret != 0){
		printf("%s(%d):pthread_join error: %s\n", __func__, __LINE__, strerror(errno));
		return -1;
	}

	debug(3, "<==\n");

	return 0;
}

/****************** 配时方案 ***************************/
/* FIXME
 * 当前执行的配时方案 */
int tsc_prog_actual(void)
{
#ifdef _SIM_TEST_
	int ret = sim_prog_actual();
	//printf("actual prog:%d\n", ret);
	return ret;
#else

#endif
}

/* FIXME
 * 选择新的配时方案 */
int tsc_prog_select(void)
{
#ifdef _SIM_TEST_
	int ret = sim_prog_select();
	//printf("select prog:%d\n", ret);
	return ret;
#else

#endif
}

/* FIXME
 * 当前配时方案，运行处于方案内的时间点
 */
int tsc_prog_tx(void)
{
#ifdef _SIM_TEST_
	int ret = sim_prog_tx();
	//printf("tx:%d\n", ret);
	return ret;
#else

#endif
}

/* FIXME
 * 当前配时方案，总的时长
 */
int tsc_prog_tu(void)
{
#ifdef _SIM_TEST_
	int ret = sim_prog_tu();
	//printf("tu:%d\n", ret);
	return ret;
#else

#endif
}

/* FIXME:当前只有本地指令，没有网络功能
 * 谁发出方案切换命令
 */
int tsc_prog_src(void)
{
#ifdef _SIM_TEST_
	return 0;
#else

#endif
}

/* FIXME
 * VSPLUS告知控制器某条交通流已等待很长时间，
 * 控制器判断是否切回定时配时方案
 */
void tsc_stream_waiting(int index, int time)
{
	debug(2, "stream:%d, waiting:%d\n", index, time);
}

/*************检测器函数************************/
/* FIXME
 * 读取检测器上升沿计数
 */
int tsc_sum_rising(int index)
{
#ifdef _SIM_TEST_
	//printf("%s(%d):det index:%d\n", __func__, __LINE__, index);
	return sim_sum_rising(index);
#else

#endif
}

/* FIXME
 * 清空检测器上升沿计数
 */
void tsc_clr_rising(int index)
{
#ifdef _SIM_TEST_
	sim_clr_rising(index);
#else

#endif
}

/* FIXME
 * 读取检测器下降沿计数
 */
int tsc_sum_falling(int index)
{
#ifdef _SIM_TEST_
	//printf("%s(%d):det index:%d\n", __func__, __LINE__, index);
	return sim_sum_falling(index);
#else

#endif
}

/* FIXME
 * 清空检测器下降沿计数
 */
int tsc_clr_falling(int index)
{
#ifdef _SIM_TEST_
	return sim_clr_falling(index);
#else

#endif
}

/* FIXME
 * 检测器占用率，单位是percent
 */
int tsc_cur_hold(int index)
{
#ifdef _SIM_TEST_
	return sim_cur_hold(index);
#else

#endif
}

/* FIXME
 * 平滑处理后的检测器占用率，单位是percent
 */
int tsc_sm_hold(int index)
{
#ifdef _SIM_TEST_
	return sim_sm_hold(index);
#else

#endif
}

/* FIXME
 * 检测器当前占用状态
 */
int tsc_hold_state(int index)
{
#ifdef _SIM_TEST_
	return sim_hold_state(index);
#else

#endif
}

/* FIXME
 * 检测器当前已占用时间
 */
int tsc_hold_time(int index)
{
#ifdef _SIM_TEST_
	return sim_hold_time(index);
#else

#endif
}

/* FIXME
 * 检测器是否出现故障 */
int tsc_det_fault(int index)
{
#ifdef _SIM_TEST_
	return sim_det_fault(index);
#else 

#endif
}

/* FIXME
 * 检测指定的检测器是否存在
 */
int tsc_det_exist(int index)
{
#ifdef _SIM_TEST_
	return sim_det_exist(index);
#else

#endif
}

/* FIXME
 * 自最后一个下降沿以来的时间间隔
 */
int tsc_det_net(int index)
{
#ifdef _SIM_TEST_
	return sim_det_net(index);
#else

#endif
}

/* FIXME
 * 自最后一个上升沿以来的时间间隔
 */
int tsc_det_gross(int index)
{
#ifdef _SIM_TEST_
	return sim_det_gross(index);
#else

#endif
}
/***************** 其他函数 *****************************/

/* FIXME
 * 判断当前vsplus控制状态。
 * vsplus关闭或切换时返回1
 * 其他返回0（vsplus动态控制，或vsplus定时控制中)
 */
int tsc_ctl_active(void)
{
	return 0;
}

/* 返回当前时间 */
int tsc_get_time(int* hour, int* min, int* sec)
{
	time_t tt = time(NULL);
	struct tm *t = localtime(&tt);
	*hour = t->tm_hour;
	*min = t->tm_min;
	*sec = t->tm_sec;
	debug(3, "%04d-%02d-%02d %02d:%02d:%02d\n", t->tm_year+1900,
		t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

	return 1;
}

/* 返回当前日期 */
int tsc_get_date(int* year, int* month, int* mday, int* wday)
{
	time_t tt = time(NULL);
	struct tm *t = localtime(&tt);
	*year = t->tm_year+1900;
	*month = t->tm_mon+1;
	*mday = t->tm_mday;
	*wday = t->tm_wday;

	return 1;
}

/***************** 信号控制函数 *****************/
//打开灯组运行信息文件
void open_sg(void)
{
	int ret;
	g_fd_sg = open("sg_nodes.dat", O_RDWR|O_CREAT, 0644);
	if(g_fd_sg < 0){
		printf("%s\n", strerror(errno));
		return;
	}
	if(ret = ftruncate(g_fd_sg, sizeof(sg_node)*SGMAX) < 0){
		printf("%s\n", strerror(errno));
	}
	g_sg = mmap(NULL, sizeof(sg_node)*SGMAX, PROT_READ|PROT_WRITE, MAP_SHARED, g_fd_sg, 0);
	if(g_sg == MAP_FAILED){
		printf("%s\n", strerror(errno));
	}
	//printf("sg:sizeof:%d\n", sizeof(sg_node)*SGMAX);

	//FIXME:实际需要从XML文件中解析初始化数据
	memset(g_sg, 0, sizeof(sg_node)*SGMAX);
	int min_red[16] = {40,40,40,40,40,40,40,80,80,80,80,40,40,40,40,60};
	int min_green[16] = {20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};
	int i;
	for(i = 0; i < 16; i++){
		g_sg[i].stat = 2;//random()%5+1;
		g_sg[i].red_min = min_red[i];
		g_sg[i].green_min = min_green[i];
		g_sg[i].prep = 30;
		g_sg[i].amber = 30;
		g_sg[i].time = 0;
		g_sg[i].ext = 0;
	}
}

void close_sg(void)
{
	close(g_fd_sg);
	g_fd_sg = 0;
}

int thr_sg(void* arg)
{
	int i;
	//xml_para* para = (xml_para*)(arg);
	xml_para* para = g_xml_para;
#ifdef _TEST_SG_
	char buf[4];//初始化所有灯组红灯
    buf[0] = 0x96;
    buf[1] = 0xff;//所有灯组
    buf[2] = 0x01;//红灯
    buf[3] = 0x69;
#endif
	while(!g_exit){
		for(i = 0; i < SGMAX; i++){
			g_sg[i].time++;
			switch(g_sg[i].stat){
//				case 0://关灯状态
//					if(g_sg[i].ext == 1){//open
//						g_sg[i].ext = 0;
//						g_sg[i].stat = 4;
//						g_sg[i].time = 0;
//#ifdef _TEST_SG_
//						buf[1] = (i/4) << 4 | (i%4);
//						buf[2] = 0x02;//黄
//						write(g_fd_serial, buf, sizeof(buf));
//#endif
//					}
//					else if(g_sg[i].ext == 2){//close
//						g_sg[i].ext = 0;
//						g_sg[i].stat = 1;
//						g_sg[i].time = 0;
//#ifdef _TEST_SG_
//						buf[1] = (i/4) << 4 | (i%4);
//						buf[2] = 0x02;//黄
//						write(g_fd_serial, buf, sizeof(buf));
//#endif
//					}
//					else
//						g_sg[i].time = 0;
//					break;

				case 1://1-amber;2-min_red;3-ex_red;4-prep;5-min_green;6-ex_green
					if(g_sg[i].time > g_sg[i].amber){
						g_sg[i].stat++;
						g_sg[i].time = 0;
#ifdef _TEST_SG_
						buf[1] = g_xml_para->det_sg[i];
						buf[2] = 0x01;//红
    					write(g_fd_serial, buf, sizeof(buf));
#endif
					}
					break;
				case 2:
					if(g_sg[i].time > g_sg[i].red_min){
						g_sg[i].stat++;
						g_sg[i].time = 0;
#ifdef _TEST_SG_
						buf[1] = g_xml_para->det_sg[i];
						buf[2] = 0x01;//红
    					write(g_fd_serial, buf, sizeof(buf));
#endif
					}
					break;
				case 3://b00状态未变，b01进行红绿切换，b10进行绿红切换
					if(g_sg[i].ext == 1){//扩展红绿灯时间不固定，根据ext判断是否结束。ext由vsplus调用的open signal函数设置。
						g_sg[i].ext = 0;
						g_sg[i].stat++;
						g_sg[i].time = 0;
#ifdef _TEST_SG_
						buf[1] = g_xml_para->det_sg[i];
						buf[2] = 0x02;//黄
    					write(g_fd_serial, buf, sizeof(buf));
#endif
					}
					break;
				case 4:
					if(g_sg[i].time > g_sg[i].prep){
						g_sg[i].stat++;
						g_sg[i].time = 0;
#ifdef _TEST_SG_
						buf[1] = g_xml_para->det_sg[i];
                        buf[2] = 0x04;//绿
                        write(g_fd_serial, buf, sizeof(buf));
#endif
					}
					break;
				case 5:
					if(g_sg[i].time > g_sg[i].green_min){
						g_sg[i].stat++;
						g_sg[i].time = 0;
#ifdef _TEST_SG_
						buf[1] = g_xml_para->det_sg[i];
                        buf[2] = 0x04;//绿
                        write(g_fd_serial, buf, sizeof(buf));
#endif
					}
					break;
				case 6:
					if(g_sg[i].ext == 2){
						g_sg[i].ext = 0;
						g_sg[i].stat = 1;
						g_sg[i].time = 0;
#ifdef _TEST_SG_
						buf[1] = g_xml_para->det_sg[i];
                        buf[2] = 0x02;//黄
                        write(g_fd_serial, buf, sizeof(buf));
#endif
					}
					break;
			}
		}
		us_sleep(100000);
	}
}

void init_sg(void)
{
	debug(3, "==>\n");
	open_sg();
	pthread_create(&g_tid_sg, NULL, thr_sg, NULL);
	debug(3, "<==\n");
}

void deinit_sg(void)
{
	debug(3, "==>\n");
	pthread_join(g_tid_sg, NULL);
	close_sg();
	debug(3, "<==\n");
}
/* FIXME
 * 测试指定的signal group是否存在
 */
int tsc_sg_exist(int sg)
{
//	return 1;
	if((sg >=1 ) && (sg <= 15))
		return 1;
	else
		return 0;
}

/* FIXME
 * 测试指定的signal group是否故障
 */
int tsc_sg_fault(int sg)
{
	if(g_sg[sg].stat == -1)
		return 1;
	else
		return 0;
}
/* FIXME
 * 测试sg是否处于vsplus控制
 */
int tsc_sg_enabled(int sg)
{
//	return 1;
	if((sg >=1 ) && (sg <= 15))
		return 1;
	else
		return 0;
}

/* FIXME
 * 打开信号灯，红->绿 */
void ts_sg_open(int sg)
{
	g_sg[sg].ext = 1;
#ifdef _TEST_SG_
	char buf[4];
	buf[0] = 0x96;
	buf[1] = (sg/4)<<4 | (sg%4);
	buf[2] = 0x02;//黄
	buf[3] = 0x69;
	write(g_fd_serial, buf, sizeof(buf));
	sleep(g_sg[sg].prep);
	buf[2] = 0x04;//绿
	write(g_fd_serial, buf, sizeof(buf));
#endif
}

/* FIXME
 * 关闭信号灯，绿->红 */
void ts_sg_close(int sg)
{
	g_sg[sg].ext = 2;
#ifdef _TEST_SG_
	char buf[4];
	buf[0] = 0x96;
	buf[1] = (sg/4)<<4 | (sg%4);
	buf[2] = 0x02;//黄
	buf[3] = 0x69;
	write(g_fd_serial, buf, sizeof(buf));
	sleep(g_sg[sg].amber);
	buf[2] = 0x01;//红
	write(g_fd_serial, buf, sizeof(buf));
#endif
}

/***** 信号灯状态检查 *****/
/* FIXME
 * 检查灯组是否处于红灯状态 */
int tsc_chk_red(int sg)
{
	//us_sleep(2000);
	if((g_sg[sg].stat == 2) || (g_sg[sg].stat == 3))
		return 1;
	else
		return 0;
}

/* FIXME
 * 检查灯组是否处于最小红灯状态 */
int tsc_chk_min_red(int sg)
{
	if(g_sg[sg].stat == 2)
		return 1;
	else
		return 0;
}

/* FIXME
 * 检查信号灯是否处于amber状态 */
int tsc_chk_amber(int sg)
{
	if(g_sg[sg].stat == 1)
		return 1;
	else
		return 0;
}

/* FIXME
 * 检查信号灯是否处于green状态 */
int tsc_chk_green(int sg)
{
	if((g_sg[sg].stat == 5) || (g_sg[sg].stat == 6))
		return 1;
	else
		return 0;
}

/* FIXME
 * 检查信号灯是否处于最小green状态 */
int tsc_chk_min_green(int sg)
{
	if(g_sg[sg].stat == 5)
		return 1;
	else
		return 0;
}

/* FIXME
 * 检查信号灯是否处于prep状态 */
int tsc_chk_prep(int sg)
{
	if(g_sg[sg].stat == 4)
		return 1;
	else
		return 0;
}

/***** 信号灯状态计时 *****/
/* FIXME
 * 返回红灯亮起时间 */
int tsc_red_time(int sg)
{
	if(g_sg[sg].stat == 2)
		return g_sg[sg].time;
	else if(g_sg[sg].stat == 3)
		return g_sg[sg].time + g_sg[sg].red_min;
	else
		return 0;
}

/* FIXME
 * 返回最小红灯亮起时间 */
int tsc_min_red_time(int sg)
{
	if(g_sg[sg].stat == 2)
		return g_sg[sg].time;
	else
		return 0;
}

/* FIXME
 * 返回绿灯时间 */
int tsc_green_time(int sg)
{
	if(g_sg[sg].stat == 5)
		return g_sg[sg].time;
	else if(g_sg[sg].stat == 6)
		return g_sg[sg].time + g_sg[sg].green_min;
	else
		return 0;
}

/* FIXME
 * 返回最小绿灯亮起时间 */
int tsc_min_green_time(int sg)
{
	if(g_sg[sg].stat == 5)
		return g_sg[sg].time;
	else
		return 0;
}

/* FIXME
 * 返回amber时间 */
int tsc_amber_time(int sg)
{
	if(g_sg[sg].stat == 1)
		return g_sg[sg].time;
	else
		return 0;
}

/* FIXME
 * 返回prep时间 */
int tsc_prep_time(int sg)
{
	if(g_sg[sg].stat == 4)
		return g_sg[sg].time;
	else
		return 0;
}

/***** 信号灯状态设定值 *****/
/* FIXME
 * 返回最小红灯时间 */
int tsc_min_red(int sg)
{
	return g_sg[sg].red_min;
}

/* FIXME
 * 返回红绿过度时间 */
int tsc_prep(int sg)
{
	return g_sg[sg].prep;
}

/* FIXME
 * 返回最小绿灯时间 */
int tsc_min_green(int sg)
{
	return g_sg[sg].green_min;
}

/* FIXME
 * 返回绿红过渡时间 */
int tsc_amber(int sg)
{
	return g_sg[sg].amber;
}

/* FIXME: 需要根据XML定义
 * 返回绿间隔时间 */
int tsc_inter_green(int sgr, int sge)
{
	return 0;//32767;
}
/******************* Digital output switching ************************/
/* FIXME:
 * 打开一个数字信号输出 */
void tsc_digital_on(int sg)
{
	//us_sleep(1);
} 

/* FIXME:
 * 关闭一个数字信号输出 */
void tsc_digital_off(int sg)
{
	//us_sleep(1);
}

/* FIXME:
 * 打开一个数字闪烁信号输出 */
void tsc_digital_blink_on(int sg)
{
	//us_sleep(1);
}

/* FIXME:
 * 关闭一个数字闪烁信号输出 */
void tsc_digital_blink_off(int sg)
{
	//us_sleep(1);
}

/* FIXME
 * 检查某个数字信号输出的状态 
 * 0 = 关闭，1 = 打开*/
int tsc_digital_state(int sg)
{
	return 0;
}

/* FIXME
 * 检查某个数字闪烁信号输出的状态 
 * 0 = 关闭，1 = 打开*/
int tsc_digital_blink_state(int sg)
{
	return 0;
}

/*********************************************************************/
/* FIXME
 * 读取PT信息？*/
int tsc_read_pt(void* arg)
{
	PTMSG* ptr = (PTMSG*)arg;
	debug(3, "call point:%d\n", ptr->MP);
	debug(3, "line:%d\n", ptr->Linie);
	debug(3, "course:%d\n", ptr->Kurs);
	debug(3, "route:%d\n", ptr->Route);
	debug(3, "priority:%d\n", ptr->Prioritaet);
	debug(3, "vehicle length:%d\n", ptr->Laenge);
	debug(3, "direction by hand:%d\n", ptr->RichtungVonHand);
	debug(3, "difference to schedule:%d\n", ptr->FahrplanAbweichnung);

	return 0;
}

/******************** 串口操作 **********************************/
int set_opt(int fd, int speed, int bits, char event, int stop)
{
	struct termios newtio, oldtio;
	if(tcgetattr(fd, &oldtio) != 0){
		debug(1, "tcgetattr fail:%s\n", strerror(errno));
		return -1;
	}
	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag |= CLOCAL|CREAD;

	newtio.c_cflag &= ~CSIZE;
	switch(bits){//数据位数
		case 7:
			newtio.c_cflag |= CS7;
			break;
		case 8:
			newtio.c_cflag |= CS8;
			break;
	}

	switch(event){
		case 'O'://奇校验
			newtio.c_cflag |= PARENB;
			newtio.c_cflag |= PARODD;
			newtio.c_iflag |= INPCK;
			break;
		case 'E'://偶校验
			newtio.c_iflag |= INPCK;
			newtio.c_iflag |= PARENB;
			newtio.c_iflag &= ~PARODD;
			break;
		case 'N':
			newtio.c_cflag &= ~PARENB;
			break;
	}

	switch(speed){
		case 2400:
			cfsetispeed(&newtio, B2400);
			cfsetospeed(&newtio, B2400);
			break;
		case 4800:
			cfsetispeed(&newtio, B4800);
			cfsetospeed(&newtio, B4800);
			break;
		case 9600:
			cfsetispeed(&newtio, B9600);
			cfsetospeed(&newtio, B9600);
			break;
		case 19200:
			cfsetispeed(&newtio, B19200);
			cfsetospeed(&newtio, B19200);
			break;
		case 115200:
			cfsetispeed(&newtio, B115200);
			cfsetospeed(&newtio, B115200);
			break;
		default:
			debug(1, "not support bit speed %d, will set to default %d\n", speed, 9600);
			cfsetispeed(&newtio, B9600);
			cfsetospeed(&newtio, B9600);
			break;
	}

	if(stop == 1){
		newtio.c_cflag &= ~CSTOPB;
	}
	else if(stop == 2){
		newtio.c_cflag |= CSTOPB;
	}

	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 0;

	tcflush(fd, TCIFLUSH);

	if((tcsetattr(fd, TCSANOW, &newtio)) != 0){
		debug(1, "set error:%s\n", strerror(errno));
		return -1;
	}

	return 0;
}

int open_port(int port)
{
	char portname[20];
	int fd;
	//sprintf(portname, "/dev/ttyS%d", port);
	sprintf(portname, "/dev/ttyUSB0", port);//FIXME:TEST
	fd = open(portname, O_RDWR|O_NOCTTY|O_NONBLOCK);//|O_NDELAY);
	if(fd == -1){
		debug(1, "cannot open %s\n", portname);
		return -1;
	}

	debug(3, "open %s sucess\n", portname);

	return fd;
}

/* 给驱动板喂狗 */
int thr_watchdog(void* para)
{
	int fd = *(int*)(para);
	char buf[3];
	buf[0] = 0xC1;
	buf[1] = 0xAB;
	buf[2] = 0x5C;
	int ret;
	while(!g_exit){
		ret = write(fd, buf, sizeof(buf));
		sleep(2);//每隔一段时间喂一次
	}

	return 0;
}

//返回0表示初始化成功
int init_serial(void)
{
	debug(3, "==>\n");

	int ret = 0;

	g_fd_serial = open_port(1);//ttyS1
	if(g_fd_serial == -1){
		debug(1, "open_port() error\n");
		return -1;
	}

	if(set_opt(g_fd_serial, 19200, 8, 'N', 1) != 0){
		debug(1, "set_opt() error\n");
		return -1;
	}

	ret = pthread_create(&g_tid_watchdog, NULL, thr_watchdog, &g_fd_serial);
	if(ret != 0){
		debug(1, "pthread_create() error:%s\n", strerror(errno));
		return -1;
	}

	debug(3, "<==\n");

	return 0;
}

void deinit_serial(void)
{
	debug(3, "==>\n");

	g_exit = 1;
	pthread_join(g_tid_watchdog, NULL);
	close(g_fd_serial);

	debug(3, "<==\n");
}


/********************* 初始化 ************************/
//如果有参数调整的需求，放在main.c中更合适
int tsc_init()
{
	int ret = 0;
	int i = 0;
	for(i = 0; i < 46; i++)
		printf("%d:%d ", i, g_xml_para->det_sg[i]);
	printf("\n");

	ret = init_serial(); //打开和底板通信的串口
	if(ret != 0){
		debug(1, "init_serial() error\n");
		return -1;
	}

	ret = init_timers();	//初始化内部定时器
	if(ret != 0){
		debug(1, "init_timers() error\n");
		return -1;
	}

	init_sg(); //初始化信号灯状态记录
#ifdef _SIM_TEST_
	sim_init(); //在sim.c中实现部分测试数据的产生
#endif

	return 0;
}

int tsc_deinit()
{
	deinit_serial();
	deinit_timers();
	deinit_sg();
#ifdef _SIM_TEST_
	sim_deinit();
#endif

	return 0;
}
