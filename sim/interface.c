/*
 * VSPLUS需要的接口。
 * 实际实现在tsc.c中，此处主要封装保持接口一致
 */
#include "tsc.h"
#include "sim_vsplus.h"
#include "interface.h"

short timer(short funktion, short timer)
{
	return _timer(funktion, timer, 0);
}

short timer_2(short funktion, short timer, short wert)
{
	return _timer(funktion, timer, wert);
}

short ProgrammAktuell(void)
{
	return program_actual();
}

short ProgrammWahl(void)
{
	return program_selected();
}
