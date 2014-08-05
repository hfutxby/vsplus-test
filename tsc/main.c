#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "if626bas.h"
#include "if626max.h"
#include "vsp626ste.h"
#include "VSP_Interface.h"
#include "tsc.h"

int main(int argc, char* argv[])
{
	int i, ret;

#if USE_TTY
	//打开串口
	if(argc != 2)
		debug(1, "tty? port not found\n");
	ret = init_serial(argv[1]);
	if(ret < 0){
		debug(1, "init_serial() failed\n");
		return -1;
	}
#endif/* USE_TTY */

	//初始化vsplus
	printf("\nvs_init()\n");
	ret = vs_init();
	if(ret < 0){
		debug(1, "vs_init() failed\n");
		return -1;
	}

#if USE_TTY
	//开始接受串口发送的指令，在此之前发送的指令丢失
	start_serial();
#else
	det_read_init();
#endif/* USE_TTY */

	//运行vsplus
	printf("\nvs_start()\n");
	ret = vs_start();
	if(ret < 0){
		debug(1, "vs_start() failed\n");
		return -1;
	}


	//记录日志
//	printf("\nvs_log()\n");
//	vs_log();

	//sleep(10);

	////暂停vsplus
	//printf("\nvs_stop\n");
	//ret = vs_stop();
	//if(ret < 0){
	//	printf("vs_stop() failed\n");
	//}

	//sleep(10);

	////运行vsplus
	//printf("\nvs_start()\n");
	//ret = vs_start();
	//if(ret < 0){
	//	debug(1, "vs_start() failed\n");
	//	return -1;
	//}

	//stop_serial();

	//vs_deinit();
//
//	sleep(20);
//	//初始化vsplus
//	printf("\nvs_init()\n");
//	ret = vs_init();
//	if(ret < 0){
//		debug(1, "vs_init() failed\n");
//		return -1;
//	}
	//运行vsplus
//	printf("\nvs_start()\n");
//	ret = vs_start();
//	if(ret < 0){
//		printf("vs_start() failed\n");
//	}
//
	while(1) 
		sleep(1);

	return 0;
}
