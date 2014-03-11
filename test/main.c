/* 模拟测试vsplus */

#include <stdio.h>
#include "tsc.h"
#include "driver.h"
#include "interface.h"
#include "if626bas.h"

int test_timer(void)
{
	init_timers();
	int func, index, count, ret;
	while(1){
		pr_info();
		scanf("%d %d %d", &func, &index, &count);
		if(func == 1){
			ret = timer(func, index, count);
			printf("timer[%d],count=%d\n", index, ret);
		}
		else{
			ret = timer(func, index, count);
		}
	}
}

int test_program(void)
{
	init_driver();
	int i = 0;
	while(1){
		i++;
		sleep(1);
		printf("program_actual:%d\n", program_actual());
		if(i%3 == 0)
			printf("program_selected:%d\n\n", program_selected());
	}

	return 0;
}

int test_vsplus(void)
{
	init_timers();
	init_driver();
}

int main(void)
{
    //test_timer();
    //test_program();
    printf("GERAET_TEILKNOTEN_MAX=%d\n", GERAET_TEILKNOTEN_MAX);
    
    return 0;
}
