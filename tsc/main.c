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
	sleep(10);


#if 0
	char img_off[SGMAX] = {};
	//img_off[1] = 1;
	printf("\nvs_switch()\n");
	ret = vs_switch(img_off);
	printf("call vs_switch ret:%d\n", ret);
	if(ret < 0){
		debug(1, "vs_switch() failed\n");
		return -1;
	}
#endif
#if 0
	printf("\nvs_stop\n");
	ret = vs_stop();
	if(ret < 0){
		debug(1, "vs_stop() failed\n");
		return -1;
	}
	sleep(1);
#endif
#if 0
	char img_on[SGMAX] = {};
	img_on[0] = 1;
	for(i = 0; i < SGMAX; i++){
		if(img_on[i])
			drv_sg_switch(i+1, 5);
	}
#endif
	sleep(5);

#if 0
	//运行vsplus
	printf("\nvs_start()\n");
	ret = vs_start();
	if(ret < 0){
		debug(1, "vs_start() failed\n");
		return -1;
	}
	sleep(1);
#endif

#if 0
	sleep(5);
	printf("\nvs_start()\n");
	ret = vs_start();
	if(ret < 0){
		debug(1, "vs_start() failed\n");
		return -1;
	}
#endif

	while(1) 
		sleep(1);

	return 0;
}
