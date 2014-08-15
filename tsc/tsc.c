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
#include <pthread.h>
#include <math.h>

#include "tsc.h"
#include "sg_track.h"
#include "prg_track.h"
//#include "sim.h"
#include "drive.h"
#include "if626bas.h"
#include "if626max.h"


/****************** 参数配置相关函数 **************************/
void* g_mem[3]; //VSPLUS内部分配用于存储参数
FILE* g_vcb_file = NULL; //配置参数vcb文件
FILE* g_vcb_back = NULL; //备份参数vcb文件
FILE* g_cmd_vcb_file = NULL;
#define VCB_FILE "para.vcb" //配置参数vcb文件
#define VCB_BACK "para_bak.vcb" //备份参数vcb文件
#define CMD_VCB "newcmd.vcb"	//command vcb file

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

/* 检查command vcb文件是否存在 */
int tsc_chk_cmd_vcb(void)
{
	if(access(CMD_VCB, F_OK) == 0)
		return 1;
	else
		return 0;
}

/* 打开command VCB文件 */
int tsc_open_cmd_vcb(void)
{
	if(g_cmd_vcb_file != NULL){
		debug(2, "cmd vcb file already opened\n");
		return 1;
	}
	else{
		g_cmd_vcb_file = fopen(CMD_VCB, "rb");
		if(g_cmd_vcb_file == NULL){
			debug(1, "%s\n", strerror(errno));
			return 0;
		}
		return 1;
	}
}

/* 关闭command VCB文件 */
void tsc_close_cmd_vcb(void)
{
	if(g_cmd_vcb_file == NULL){
		debug(2, "cmd vcb file not opened\n");
		return;
	}
	else{
		if(fclose(g_cmd_vcb_file) != 0){
			debug(1, "%s\n", strerror(errno));
		}
		//remove(CMD_VCB);
		rename(CMD_VCB, "oldcmd.vcb");
		g_cmd_vcb_file = NULL;
	}
}

/* 读配置command VCB数据 */
int tsc_read_cmd_vcb(char *data, int size)
{
	if(g_cmd_vcb_file == NULL){
		debug(1, "cmd vcb file not opened\n");
		return -1;
	}
	memset(data, 0, size);
	return fread(data, 1, size, g_cmd_vcb_file);

}
/****************** 定时器 ***************************/
static int g_timer[MAXTIMER];//最低位做开关标志
pthread_t g_tid_timer;//定时器计时线程
pthread_mutex_t mutex_timer = PTHREAD_MUTEX_INITIALIZER;
static int g_exit_timer = 0;

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

void* thr_timer(void* arg)
{
	debug(3, "==>\n");
	struct timeval tv1, tv2;
	long us;
	while(!g_exit_timer){
		gettimeofday(&tv1, NULL);
		pthread_mutex_lock(&mutex_timer);
		time_go();
		pthread_mutex_unlock(&mutex_timer);
		gettimeofday(&tv2, NULL);
		us = (tv2.tv_sec - tv1.tv_sec) * 1000 * 1000 + (tv2.tv_usec - tv1.tv_usec);
		us_sleep(100 * 1000 - us);
	}
	debug(3, "<==\n");
}

/* 定时器 */
int tsc_timer(int func, int index, int count)
{
	//debug(3, "==>\n");
	pthread_mutex_lock(&mutex_timer);
	int ret = 0;
	int i = 0;
	if((index >= 0) && (index < MAXTIMER)){
		switch(func){
			case 1://read
				ret = g_timer[index]>>1;
				break;
			case 2://load and start
				g_timer[index] = (count<<1) + 1;
				ret = g_timer[index]>>1;
				break;
			case 3://clear
				g_timer[index] = 1;
				ret = g_timer[index]>>1;
				break;
			case 4://stop and clear
				g_timer[index] = 0;
				ret = g_timer[index]>>1;
				break;
			case 5://stop
				g_timer[index] &= 0xfffffffe;
				ret = g_timer[index]>>1;
				break;
			case 6://test
				debug(2, "test\n");
				for(i = 0; i < MAXTIMER; i++){
					printf("timer[%d],%s,count=%d\n", i, (g_timer[i]%2) ? "running" : "stopped", g_timer[i]>>1);
				}
				break;
		}
	}

	pthread_mutex_unlock(&mutex_timer);
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
		debug(1, "pthread_create error: %s\n", __func__, __LINE__, strerror(errno));
		return -1;
	}

	debug(3, "<==\n");

	return 0;
}

void deinit_timers(void)
{
	debug(3, "==>\n");
	g_exit_timer = 1;
	
	if(g_tid_timer)
		pthread_join(g_tid_timer, NULL);

	debug(2, "<==\n");
}

/****************** 配时方案 ***************************/
static prg_def* g_prg = NULL;

int init_tsc_prg(void)
{
	debug(3, "==>\n");
	int size = sizeof(prg_def) * PRGMAX;
	g_prg = (prg_def*)malloc(size);
	if(!g_prg){
		debug(1, "malloc fail\n");
		return -1;
	}
	drv_prg_para(g_prg, size);
	//drv_prg_para_dump(g_prg, size);
	debug(3, "<==\n");

	return 0;
}

void deinit_tsc_prg(void)
{
	if(g_prg)
		free(g_prg);
	debug(2, "<==\n");
}

/* 
 * 当前执行的配时方案 */
int tsc_prog_actual(void)
{
	int ret;
	ret = prg_track_cur();

	return ret;
}

/* 
 * 选择新的配时方案 */
int tsc_prog_select(void)
{
	int ret;
	ret = prg_track_next();
	debug(2, "ret:%d\n", ret);

	return ret;
}

/* 
 * 当前配时方案，运行处于方案内的时间点
 */
int tsc_prog_tx(void)
{
	int ret;
	ret = prg_track_tx();

	return ret;
}

/* 
 * 当前配时方案，总的时长
 */
int tsc_prog_tu(void)
{
	int ret;
	int index = prg_track_cur();
	ret = g_prg[index].tu;

	return ret;
}

/* FIXME:当前只有本地指令，没有网络功能
 * 谁发出方案切换命令
 */
int tsc_prog_src(void)
{
	return 1;
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
static double g_f1 = 0.5;//占用率上升折算因子
static double g_f2 = 0.125;//下降因子
#define DET_MAXTIME 600 //等待下降沿超时
#define TEST_ID 5

static det_track* g_det = NULL; //检测器数据
static int g_fd_det = 0; //检测器数据映射文件
static det_def* g_det_def = NULL; //检测器配置

static int g_exit_det = 0; //检测器数据维护线程退出标志
static pthread_mutex_t mutex_det = PTHREAD_MUTEX_INITIALIZER;
static pthread_t g_tid_det;

/* 打开检测器数据映射 */
int open_det(void)
{   
    int i, ret;
    //默认保存文件
    g_fd_det = open("det_nodes.dat", O_RDWR|O_CREAT, 0644);
    if(g_fd_det < 0){
        debug(1, "%s\n", strerror(errno));
        return -1;
    }
    if((ret = ftruncate(g_fd_det, sizeof(det_track) * DETMAX)) < 0){
        debug(1, "%s\n", strerror(errno));
		return -1;
    }
    //映射进内存
    g_det = mmap(NULL, sizeof(det_track) * DETMAX,  PROT_READ|PROT_WRITE, MAP_SHARED, g_fd_det, 0);
    if(g_det ==  MAP_FAILED){
        debug(1, "%s\n", strerror(errno));
		return -1;
    }
    memset(g_det, 0, sizeof(det_track) * DETMAX);
	for(i = 0; i < DETMAX; i++){
		g_det[i].free = 10;//即时占用率计算窗口为1s
		g_det[i].rh = alloc_ring(11);
	}

	return 0;
}

/* 关闭检测器数据映射 */
void close_det(void)
{
	int i;
	for(i = 0; i < DETMAX; i++){
		free_ring(g_det[i].rh);
		g_det[i].rh = NULL;
	}
	munmap(g_det, sizeof(det_track) * DETMAX);
    close(g_fd_det);
    g_fd_det = 0;
}


void update_det(void)
{
	int i;
	for(i = 0; i < DETMAX; i++){
		if(g_det[i].fault || !g_det_def[i].exist)//检测器故障或不存在，不用更新数据
			continue;

		if(g_det[i].state){//检测器占用中
			g_det[i].gross++;
			ring_add_over(g_det[i].rh, 1);
		}
		else{
			g_det[i].gross++;
			g_det[i].net++;
			ring_add_over(g_det[i].rh, 0);
		}


		//计算占用率
		g_det[i].occ1 = (ring_sum(g_det[i].rh) * 10);
		if(g_det[i].occ1 > g_det[i].occ2)
			g_det[i].occ2 = g_det[i].occ2 + g_f1 * (g_det[i].occ1 - g_det[i].occ2);
		else
			g_det[i].occ2 = g_det[i].occ2 + g_f2 * (g_det[i].occ1 - g_det[i].occ2);
	}
}

/* 驱动模块调用此函数，op=1上升沿,op=2下降沿 */
void tsc_det_op(int index, int op)
{
	debug(3, "==>\n");
	//debug(2, "==>index:%d, op:%d\n", index, op);
	pthread_mutex_lock(&mutex_det);
	if(op == 1){
		g_det[index].sum_rising++;
		g_det[index].state = 1;
		g_det[index].net = 0;
		g_det[index].gross = 0;
	}
	if(op == 2){
		g_det[index].sum_falling++;
		g_det[index].state = 0;
		g_det[index].net = 0;
	}
	pthread_mutex_unlock(&mutex_det);
#if USE_INI
	drv_add_det(index, (op == 1) ? 1 : 0);
#endif/* USE_INI */
	//debug(2, "<==\n");
}

/* 定时更新检测器数据 */
void* thr_det(void* arg)
{
	struct timeval tv1, tv2;
	long us;
	int i;
	while(!g_exit_det){
		gettimeofday(&tv1, NULL);
		pthread_mutex_lock(&mutex_det);
		update_det();
		pthread_mutex_unlock(&mutex_det);
		//i = 2;
		//debug(1, "det %2d occ1 %2.3f~%2d occ2 %2.3f~%2d\n", i, g_det[i].occ1, (int)g_det[i].occ1, g_det[i].occ2, (int)round(g_det[i].occ2));
		for(i = 0; i < DETMAX; i++){
			if(g_det_def[i].exist && g_det[i].state && (g_det[i].gross > DET_MAXTIME))
				tsc_det_op(i, 2);//超时，人为产生一个下降沿
		}
		gettimeofday(&tv2, NULL);
		us = (tv2.tv_sec - tv1.tv_sec) * 1000 * 1000 + (tv2.tv_usec - tv1.tv_usec);
		us_sleep(100 * 1000 - us);//100ms更新一次检测器状态
	}
}

//初始化检测器
int init_det(void)
{
	debug(3, "==>\n");
	int ret = open_det();//分配空间用于det状态跟踪
	if(ret < 0){
		debug(1, "call open_det() error\n");
		return -1;
	}
	
	//分配空间用于读取det配置
	int size = sizeof(det_def) * DETMAX;
	g_det_def = malloc(size);
	drv_det_para(g_det_def, size);

	//开始处理det数据
	pthread_create(&g_tid_det, NULL, thr_det, NULL);
	debug(3, "<==\n");

	return 0;
}

void deinit_det(void)
{
	g_exit_det = 1;
	if(g_tid_det)
		pthread_join(g_tid_det, NULL);
	
	if(g_det_def)
		free(g_det_def);

	close_det();
	debug(2, "<==\n");
}

/* 读取检测器上升沿计数 */
int tsc_sum_rising(int index)
{
	int ret;
	pthread_mutex_lock(&mutex_det);
	ret = g_det[index].sum_rising;
	pthread_mutex_unlock(&mutex_det);
	return ret;
}

/* 清空检测器上升沿计数 */
void tsc_clr_rising(int index)
{
	int ret = 0;
	pthread_mutex_lock(&mutex_det);
	g_det[index].sum_rising = 0;
	pthread_mutex_unlock(&mutex_det);
}

/* 读取检测器下降沿计数 */
int tsc_sum_falling(int index)
{
	int ret;
	pthread_mutex_lock(&mutex_det);
	ret = g_det[index].sum_falling;
	pthread_mutex_unlock(&mutex_det);
	return ret;
}

/* 清空检测器下降沿计数 */
int tsc_clr_falling(int index)
{
	int ret = 0;
	pthread_mutex_lock(&mutex_det);
	g_det[index].sum_falling = 0;
	pthread_mutex_unlock(&mutex_det);
	return 0;
}

/* 检测器占用率，单位是percent */
int tsc_cur_hold(int index)
{
	int ret;
	pthread_mutex_lock(&mutex_det);
	ret = (int)g_det[index].occ1;
	pthread_mutex_unlock(&mutex_det);
	return ret;
}

/* 平滑处理后的检测器占用率，单位是percent */
int tsc_sm_hold(int index)
{
	int ret;
	pthread_mutex_lock(&mutex_det);
	ret = (int)(round(g_det[index].occ2));
	pthread_mutex_unlock(&mutex_det);
	return ret;
}

/* 检测器当前占用状态 */
int tsc_hold_state(int index)
{
	int ret;
	pthread_mutex_lock(&mutex_det);
	ret = g_det[index].state;
	pthread_mutex_unlock(&mutex_det);
	return ret;
}

/* FIXME
 * 检测器当前已占用时间
 */
int tsc_hold_time(int index)
{
	int ret;
	pthread_mutex_lock(&mutex_det);
	if(g_det[index].state)
		ret = g_det[index].gross;
	else
		ret = 0;
	pthread_mutex_unlock(&mutex_det);
	return ret;
}

/* 检测器是否出现故障 */
int tsc_det_fault(int index)
{
	int ret;
	pthread_mutex_lock(&mutex_det);
	ret = g_det[index].fault;
	pthread_mutex_unlock(&mutex_det);
	return ret;
}

/* FIXME 
 * 检测指定的检测器是否存在
 */
int tsc_det_exist(int index)
{
	int ret;
	ret = g_det_def[index].exist;
	return ret;
}

/* 自最后一个下降沿以来的时间间隔 */
int tsc_det_net(int index)
{
	int ret;
	pthread_mutex_lock(&mutex_det);
	ret = g_det[index].net;
	pthread_mutex_unlock(&mutex_det);
	return ret;
}

/* 自最后一个上升沿以来的时间间隔 */
int tsc_det_gross(int index)
{
	int ret;
	pthread_mutex_lock(&mutex_det);
	ret = g_det[index].gross;
	pthread_mutex_unlock(&mutex_det);
	return ret;
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
	*wday = (t->tm_wday + 6) % 7 + 1;
	debug(3, "%04d-%02d-%02d,%d\n", *year, *month, *mday, *wday);

	return 1;
}

/***************** 信号控制函数 *****************/
static sg_def* g_sg = NULL;
static int g_exit_sg = 0;
static pthread_t g_tid_sg;

void* thr_sg(void* arg)
{
	int i, ret;
	long us;
	struct timeval tv1, tv2;
	while(!g_exit_sg){
		gettimeofday(&tv1, NULL);
		//open,close操作
		for(i = 0; i < SGMAX; i++){
			if(!g_sg[i].exist)//信号灯不存在
				continue;

			switch(g_sg[i].ext){
				case 1://open操作中
					if(g_sg[i].prep)
						drv_sg_switch(i, 4);//切换为prep
					else
						drv_sg_switch(i, 5);//prep=0
					g_sg[i].ext = 0;//open相应完成
					break;

				case 2://close操作中
					if(g_sg[i].green_blink)
						drv_sg_switch(i, 7);//切换为green_blink
					else
						drv_sg_switch(i, 1);
					g_sg[i].ext = 0;
					break;

				default:
					g_sg[i].ext = 0;
			}
		}
		//amber->min->ext操作
		for(i = 0; i < SGMAX; i++){
			if(!g_sg[i].exist)//信号灯不存在
				continue;

			if((ret = sg_track_chk(i, 7)) != -1){//处于green_blink
				if(ret >= g_sg[i].green_blink){//green_blink超时
					if(g_sg[i].amber)
						drv_sg_switch(i, 1);//切换到amber
					else
						drv_sg_switch(i, 2);//amber=0
					continue;
				}
			}

			if((ret = sg_track_chk(i, 1)) != -1){//处于amber
				if(ret >= g_sg[i].amber){//amber超时
					drv_sg_switch(i, 2);//切换到min_red
					continue;
				}
			}

			if((ret = sg_track_chk(i, 2)) != -1){//处于min_red
				if(ret >= g_sg[i].min_red){//min_red超时
					drv_sg_switch(i, 3);//切换到ext_red
					continue;
				}
			}

			if((ret = sg_track_chk(i, 4)) != -1){//处于prep
				if(ret >= g_sg[i].prep){//prep超时
					drv_sg_switch(i, 5);//切换到min_green
					continue;
				}
			}

			if((ret = sg_track_chk(i, 5)) != -1){//处于min_green
				if(ret >= g_sg[i].min_green){//min_green超时
					drv_sg_switch(i, 6);//切换到ext_green
					continue;
				}
			}
		}
		gettimeofday(&tv2, NULL);
		us = (tv2.tv_sec - tv1.tv_sec) * 1000 * 1000 + (tv2.tv_usec - tv1.tv_usec);
		us_sleep(100 * 1000 - us);
	}
}

/* 打开信号灯，红->绿 */
void tsc_sg_open(int sg)
{
	g_sg[sg].ext = 1;
}
 
/* 关闭信号灯，绿->红 */
void tsc_sg_close(int sg)
{
	g_sg[sg].ext = 2;
}

int init_tsc_sg(void)
{
	debug(3, "==>\n");
	int ret = -1;
	int size = SGMAX * sizeof(sg_def);
	g_sg = (sg_def*)malloc(size);
	if(!g_sg){
		debug(1, "malloc fail\n");
		return -1;
	}
	ret = drv_sg_para(g_sg, size);//读入参数
	//drv_sg_para_dump(g_sg, size);

	int i;
	for(i = 0; i < SGMAX; i++){
		if(g_sg[i].exist)
			drv_sg_switch(i, 2);//FIXME:amber
	}
	pthread_create(&g_tid_sg, NULL, thr_sg, NULL);
	debug(3, "<==\n");

	return ret;
}

void deinit_tsc_sg(void)
{
	g_exit_sg = 1;
	if(g_tid_sg)
		pthread_join(g_tid_sg, NULL);
	free(g_sg);
	debug(2, "<==\n");
}

/***** 信号灯测试函数 *****/
/* 测试指定的signal group是否存在 */
int tsc_sg_exist(int sg)
{
	return g_sg[sg].exist;
}

/* 测试指定的signal group是否故障 */
int tsc_sg_fault(int sg)
{
	return sg_track_fault(sg);
}

/* FIXME
 * 测试sg是否处于vsplus控制
 */
int tsc_sg_enabled(int sg)
{
	return 1;
	//if((sg >=1 ) && (sg <= 15))
	//	return 1;
	//else
	//	return 0;
}

/* 检查灯组是否处于红灯状态 */
int tsc_chk_red(int sg)
{
	//amber(+green_blink) + min_red + ext_red
    int ret = (sg_track_chk(sg, 1) != -1) || (sg_track_chk(sg, 2) != -1) || (sg_track_chk(sg, 3) != -1) || (sg_track_chk(sg, 7) != -1);
    if(ret)
        return 1;
    else
        return 0;
}

/* 检查灯组是否处于最小红灯状态 */
int tsc_chk_min_red(int sg)
{
    int ret = (sg_track_chk(sg, 2) != -1);
    if(ret)
        return 1;
    else
        return 0;
}

/* 检查信号灯是否处于amber状态 */
int tsc_chk_amber(int sg)
{
	//amber(+green_blink)
    int ret = (sg_track_chk(sg, 1) != -1) || (sg_track_chk(sg, 7) != -1);
    if(ret)
        return 1;
    else
        return 0;
}

/* 检查信号灯是否处于green状态 */
int tsc_chk_green(int sg)
{
    int ret = (sg_track_chk(sg, 4) != -1) || (sg_track_chk(sg, 5) != -1) || (sg_track_chk(sg, 6) != -1);
    if(ret)
        return 1;
    else
        return 0;
}

/* 检查信号灯是否处于最小green状态 */
int tsc_chk_min_green(int sg)
{
    int ret = sg_track_chk(sg, 5);
    if(ret != -1)
        return 1;
    else
        return 0;
}

/* 检查信号灯是否处于prep状态 */
int tsc_chk_prep(int sg)
{
    int ret = sg_track_chk(sg, 4);
    if(ret != -1)
        return 1;
    else
        return 0;
}

/***** 信号灯状态计时 *****/
/* 返回红灯亮起时间 */
int tsc_red_time(int sg)
{
    int ret;

	ret = sg_track_chk(sg, 7);//green_blink属于close状态
	if(ret != -1)
		return ret;

	ret = sg_track_chk(sg, 1);//红灯计时包括amber
	if(ret != -1)
		return ret + g_sg[sg].green_blink;

    ret = sg_track_chk(sg, 2);
    if(ret != -1)//min_red
        return ret + g_sg[sg].green_blink + g_sg[sg].amber;

    ret = sg_track_chk(sg, 3);
    if(ret != -1)//ext_red
        return ret + g_sg[sg].green_blink + g_sg[sg].amber + g_sg[sg].min_red;

    return 0;
}

/* 返回最小红灯亮起时间 */
int tsc_min_red_time(int sg)
{
    int ret;
    ret = sg_track_chk(sg, 2);
    if(ret != -1)
        return ret;
    else
        return 0;
}

/* 返回绿灯时间 */
int tsc_green_time(int sg)
{
    int ret;

	ret = sg_track_chk(sg, 4);//绿灯计时包括prep
	if(ret != -1)
		return ret;

    ret = sg_track_chk(sg, 5);
    if(ret != -1)//min_green
        return ret + g_sg[sg].prep;

    ret = sg_track_chk(sg, 6);
    if(ret != -1)//ext_green
        return ret + g_sg[sg].min_green + g_sg[sg].prep;

    return 0;
}

/* 返回最小绿灯亮起时间 */
int tsc_min_green_time(int sg)
{
    int ret;
    ret = sg_track_chk(sg, 5);
    if(ret != -1)
        return ret;
    else
        return 0;
}

/* 返回amber时间 */
int tsc_amber_time(int sg)
{
	int ret;
	ret = sg_track_chk(sg, 7);
	if(ret != -1)
		return ret;

	ret = sg_track_chk(sg, 1);
	if(ret != -1)
		return ret + g_sg[sg].green_blink;

	return 0;
}

/* 返回prep时间 */
int tsc_prep_time(int sg)
{
    int ret;
    ret = sg_track_chk(sg, 4);
    if(ret != -1)
        return ret;
    else
        return 0;
}

/***** 信号灯状态设定值 *****/
/* 返回最小红灯时间 */
int tsc_min_red(int sg)
{
    return g_sg[sg].min_red;
}

/* 返回红绿过度时间 */
int tsc_prep(int sg)
{
    return g_sg[sg].prep;
}

/* 返回最小绿灯时间 */
int tsc_min_green(int sg)
{
    return g_sg[sg].min_green;
}

/* 返回绿红过渡时间 */
int tsc_amber(int sg)
{
    return g_sg[sg].amber;
}

/* FIXME: 需要根据XML定义
 * 返回绿间隔时间 */
int tsc_inter_green(int sgr, int sge)
{
	return drv_inter_green(sgr, sge);
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

/********************* 初始化 ************************/
int __tsc_init()
{
    int ret = 0;

    ret = init_timers();    //初始化内部定时器
    if(ret != 0){
        debug(1, "init_timers() error\n");
        return -1;
    }

	ret = init_tsc_prg();
	if(ret != 0){
		debug(1, "init_tsc_prg() error\n");
		return -1;
	}

    ret = init_tsc_sg(); //初始化信号灯状态记录
    if(ret != 0){
        debug(1, "init_tsc_sg() error\n");
        return -1;
    }

	ret = init_det(); //初始化检测器信号跟踪记录
	if(ret != 0){
		debug(1, "init_det() error\n");
		return -1;
	}

    return 0;
}

void __tsc_deinit()
{
    deinit_det();

    deinit_tsc_sg();

	deinit_tsc_prg();

	deinit_timers();
}


