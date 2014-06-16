#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "if626bas.h"
#include "if626max.h"
#include "vsp626ste.h"
#include "VSP_Interface.h"
#include "tsc.h"

int main(void)
{
	int i, ret;
	printf("\nvs_init()\n");
	ret = vs_init();
	if(ret < 0){
		printf("vs_init() failed\n");
	}

	printf("\nvs_start()\n");
	ret = vs_start();
	if(ret < 0){
		printf("vs_start() failed\n");
	}

	sleep(1);
	vs_test();

	sleep(5);

	printf("\nvs_stop\n");
	ret = vs_stop();
	if(ret < 0){
		printf("vs_stop() failed\n");
	}

	sleep(5);

	printf("\nvs_start()\n");
	ret = vs_start();
	if(ret < 0){
		printf("vs_start() failed\n");
	}

	sleep(5);
	//while(1) 
	//	sleep(1);

	return 0;
}
