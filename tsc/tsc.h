#ifndef __TSC_H
#define __TSC_H

#if DEBUG
#define debug(level, fmt, args...) \
if(DEBUG==level) \
printf("\033[40;33m(%s:%d)\033[0m"fmt, __func__, __LINE__, ##args);
#else
#define debug(level, fmt, args...)
#endif

#endif /*__TSC_H*/
