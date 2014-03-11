/*
 * 提供VSPLUS需要的接口。
 */
#include "tsc.h"

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
