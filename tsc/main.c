#include <stdio.h>
#include <stdlib.h>
#include "if626bas.h"
#include "if626max.h"
#include "vsp626ste.h"
#include "VSP_Interface.h"
#include "tsc.h"


int main(void)
{
	int ret = 0;
#if 1
	ret = Initial_VSP_Parameter();
	debug(1, "Initial_VSP_Parameter ret=%d\n", ret);
#endif
#if 10
	ret = Pruefen_VSP_Parameter();
	debug(1, "Pruefen_VSP_Parameter ret=%d\n", ret);
#endif
#if 10
	ret = Lesen_VSP_Parameter();
	debug(1, "Lesen_VSP_Parameter ret=%d\n", ret);
#endif

	return 0;
}
