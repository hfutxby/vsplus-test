#ifndef __TSC_H
#define __TSC_H

/* 出错信息 */
#if(DEBUG>=1)
#define debug1(fmt, args...) \
printf("\033[40;31m(%s:%d)\033[0m"fmt, __func__, __LINE__, ##args) 
#else
#define debug1(fmt, args...) 
#endif /* DEBUG>=1 */

/* 警告信息 */
#if(DEBUG>=2)
#define debug2(fmt, args...) \
printf("\033[40;32m(%s:%d)\033[0m"fmt, __func__, __LINE__, ##args) 
#else
#define debug2(fmt, args...) 
#endif /* DEBUG>=2 */

/* 调试信息 */
#if(DEBUG>=3)
#define debug3(fmt, args...) \
printf("\033[40;33m(%s:%d)\033[0m"fmt, __func__, __LINE__, ##args) 
#else
#define debug3(fmt, args...) 
#endif /* DEBUG>=3 */

#define debug(level, fmt, args...) debug##level(fmt, ##args)

void* tsc_alloc_mem(int size, int id);
void tsc_free_mem(int id);
void* tsc_get_mem(int id);
int tsc_open_vcb(void);
void tsc_close_vcb(void);
int tsc_read_vcb(char *data, int size);
int tsc_prog_actual(void);
int tsc_prog_select(void);
int tsc_prog_tx(void);
int tsc_prog_tu(void);
void tsc_stream_waiting(int index, int time);

#define MAXCOUNT (32767<<1)

#endif /*__TSC_H*/
