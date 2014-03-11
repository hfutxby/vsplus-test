/*
 * 产生模拟驱动信号，测试用
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "tsc.h"

#define PRPENDLEN 6
pthread_t g_tid_program;
int g_program_list[PRPENDLEN];//方案列表，[0]为当前方案号，-1表示无方案号
extern g_exit;

void print_list(void)
{
	int i = 0;
	for(i = 0; i < PRPENDLEN; i++)
		printf("[%d]=%3d\n", i, g_program_list[i]);
}

int thr_drive_program(void* arg)
{
	int t, i;
	while(!g_exit){
		srandom(time(NULL));
		t = random()%10+1;
		printf("t=>%d\n", t);
		print_list();
		sleep(t);
		for(i = 1; i < PRPENDLEN; i++){
			if(g_program_list[i] == -1){
				g_program_list[i] = random()%255;
				break;
			}
		}
	}
	printf("g_exit\n");
}

int drive_program(void)
{
	int i = 0;
	for(i = 0; i < PRPENDLEN; i++){
		g_program_list[i] = -1;
	}
	g_program_list[0] = 0;

	pthread_create(&g_tid_program, NULL, thr_drive_program, NULL);

	return 0;
}

int drive_program_actual(void)
{
	return g_program_list[0];
}

void lshift_program_list(void)
{
	int i;
	for(i = 0; i < PRPENDLEN-1; i++)
		g_program_list[i] = g_program_list[i+1];
	g_program_list[PRPENDLEN-1] = -1;
}

int drive_program_selected(void)
{
	if(g_program_list[1] != -1)
		lshift_program_list();

	return g_program_list[0];
}

int init_driver(void)
{
	drive_program();
}

int deinit_driver(void)
{
	pthread_join(g_tid_program, NULL);
}
