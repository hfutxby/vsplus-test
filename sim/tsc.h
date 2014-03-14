#ifndef __TSC_H_
#define __TSC_H_

#define MAXCOUNT 32767<<1

int init_timers(void);
int deinit_timers(void);
int _timer(int func, int index, int count);
int program_actual(void);
int program_selected(void);

int init_tsc(void);//初始化主控
int deinit_tsc(void);
#endif /*__TSC_H_*/
