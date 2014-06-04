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

#include "tsc.h"
#include "if626bas.h"
#include "if626max.h"

int g_fd_xml_para = 0;

void* open_xml_para(void)
{
    int ret;
	xml_para* para = NULL; //从xml文件解析出来的配置参数
    g_fd_xml_para = open("xml_para.dat", O_RDWR|O_CREAT, 0644);
    if(g_fd_xml_para < 0){
        debug(1, "open error:%s\n", strerror(errno));
        return NULL;
    }
    if(ret = ftruncate(g_fd_xml_para, sizeof(xml_para)) < 0){
        printf("%s\n", strerror(errno));
    }
    para = mmap(NULL, sizeof(xml_para), PROT_READ|PROT_WRITE, MAP_SHARED, g_fd_xml_para, 0);
    if(para == MAP_FAILED){
        debug(1, "mmap error:%s\n", strerror(errno));
		return NULL;
    }

	return para;
}

void parse_xml(xml_para* para)
{
	int det_sg[46] = {0,
		1,1,1,1,//D1
		2,2,2,//D2
		3,3,3,3,//D3
		4,4,4,4,//D4
		5,5,5,//D5
		6,6,6,6,//D6
		7,8,9,10,//F7-F10
		11,11,11,12,12,12,//FL11-FL12
		0,0,0,0,0,0,//DR1-6
		13,13,13,//D13
		14,14,14,//D14
		15};//F15

	int sg[16] = {0,//匹配信号灯测试面板
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


	int i;
	for(i = 0; i < 46; i++)
		para->det_sg[i] = det_sg[i];
	for(i = 0; i < 16; i++)
		para->sg[i] = sg[i];
}

void close_xml_para(void)
{
    close(g_fd_xml_para);
    g_fd_xml_para = 0;
}

