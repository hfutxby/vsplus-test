#ifndef __TSC_H_
#define __TSC_H_

#define MAXTIMER 1912
#define MAXCOUNT 32767<<1

//#define DEBUG
#define d_printf(fmt, args...)  printf("\033[40;33m(%s:%d)\033[0m"fmt, __func__, __LINE__, ##args);
//level=1,track; level=2,watch;
#if DEBUG
#define debug(level, fmt, args...) \
if(DEBUG==level) \
printf("\033[40;33m(%s:%d)\033[0m"fmt, __func__, __LINE__, ##args);
#else
#define debug(level, fmt, args...)
#endif

int init_timers(void);
int deinit_timers(void);
int _timer(int func, int index, int count);
int program_actual(void);
int program_selected(void);

int init(void);
int deinit(void);
#endif /*__TSC_H_*/
