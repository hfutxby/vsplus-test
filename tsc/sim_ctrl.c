#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>

#include "if626max.h"
#include "sim.h"
#include "tsc.h"

static int g_exit = 0;
int g_fd_ctrl;//对vsplus运行进行部分控制测试

//struct ctrl_data{
//	int prg_cur;//当前运行的配时方案
//	int prg_next;//等待切换运行的配时方案
//}ctrl_data;

ctrl_data *g_ctrl;

int us_sleep(long us)
{
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = us;
	return select(0, NULL, NULL, NULL, &tv);
}

/* 打开一般模拟信息控制参数 */
void open_ctrl(void)
{
	int ret;
	g_fd_ctrl = open("ctrl.dat", O_RDWR|O_CREAT, 0644);
	if(g_fd_ctrl < 0){
		printf("%s\n", strerror(errno));
		return;
	}
	if(ret = ftruncate(g_fd_ctrl, sizeof(ctrl_data)) < 0){
		printf("%s\n", strerror(errno));
	}
	g_ctrl = mmap(NULL, sizeof(ctrl_data),  PROT_READ|PROT_WRITE, MAP_SHARED, g_fd_ctrl, 0);
	if(g_ctrl ==  MAP_FAILED){
		printf("%s\n", strerror(errno));
	}
//	memset(g_ctrl, 0, sizeof(ctrl_data));
//	printf("sizeof:%d\n",sizeof(ctrl_data));
//	g_ctrl->prg_cur = 1;
//	g_ctrl->prg_next = -1;
}

void close_ctrl(void)
{
	close(g_fd_ctrl);
}

/* 选择控制内容 */
int test_menu(void)
{
	int s;
	printf("select test item\n");
	printf("1:set next prg\n");
	printf("2:show prg list\n");
	printf("input item:");
	scanf("%d", &s);
	if((s >= 1) && (s <= 2))
		return s;
	else{
		printf("wrong select\n");
		return -1;
	}
}

/* item1控制实现 */
void test_1(void)
{
	int s1;
	printf("current prg:%d, next prg:%d, set next prg:", g_ctrl->prg_cur, g_ctrl->prg_next);
	scanf("%d", &s1);
	if((s1 >= 0) && (s1 <= 255)){
		g_ctrl->prg_next = s1;
		printf("set successed! current prg:%d, next prg:%d, set next prg:\n", g_ctrl->prg_cur, g_ctrl->prg_next);
	}
	else
		printf("wrong prg num\n");
}

void test_2(void)
{
	printf("current prg:%d, next prg:%d\n", g_ctrl->prg_cur, g_ctrl->prg_next);
}

/* 控制测试 */
void test(void)
{
	int item = 0;
	while(1){
		item = test_menu();
		switch(item){
		case 1:
			test_1();
			break;
		case 2:
			test_2();
			break;
		default:
			continue;
		}
	}
}

int main(void)
{
	open_ctrl();

	test();

	close_ctrl();
	return 0;
}
