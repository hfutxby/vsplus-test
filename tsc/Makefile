

ifeq ($(target),arm)
CC = arm-none-linux-gnueabi-gcc
target = arm
else
CC = gcc
target = x86
endif

#CFLAGS += -D_EMULATOR_ -D_VSPLUS_SIM_ -D_PROZESSOR_INTEL_ -D_HELPER_MACROS_ -D_ADVANCED_MEMORY_
CFLAGS += -D_VSPLUS_SIM_ -D_PROZESSOR_INTEL_ -D_HELPER_MACROS_ -D_ADVANCED_MEMORY_ -DDEBUG=3

all:main.c tsc.c VSP_Interface.c
	#$(CC) $(CFLAGS) test.c VSP_Interface.c vsp_core.lib -ldl -o test.$(target)
	#$(CC) $(CFLAGS) main.c VSP_Interface.c -L/home/byxu/workspace/vsplus/test -lvsp_core -o test.$(target)
	$(CC) $(CFLAGS) main.c tsc.c sim.c VSP_Interface.c libvsp_core.a -pthread -o main.$(target)

test: test.c tsc.c
	$(CC) $(CFLAGS) test.c tsc.c sim.c -pthread -o test.x86

clean:
	$(RM) *.x86 *.arm *.o *.out
