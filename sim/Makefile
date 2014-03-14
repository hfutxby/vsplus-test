

ifeq ($(target),arm)
CC = arm-none-linux-gnueabi-gcc
target = arm
else
CC = gcc
target = x86
endif

CFLAGS += -D_EMULATOR_ -D_VSPLUS_SIM_ -D_PROZESSOR_INTEL_ -D_HELPER_MACROS_ -D_ADVANCED_MEMORY_

test:
	$(CC) $(CFLAGS) test.c -L../vsp_core.lib -ldl -o test.$(target)

clean:
	$(RM) *.x86 *.arm *.o
