

ifeq ($(target),arm)
CC = arm-none-linux-gnueabi-gcc
target = arm
else
CC = gcc
target = x86
endif

#CFLAGS += -D_EMULATOR_ -D_VSPLUS_SIM_ -D_PROZESSOR_INTEL_ -D_HELPER_MACROS_
CFLAGS += -D_EMULATOR_ -D_VSPLUS_SIM_ -D_PROZESSOR_INTEL_ -D_HELPER_MACROS_
SRCS := timer.c tsc.c
OBJS := $(basename $(SRCS))

test:
#	$(CC) timer.c -pthread -DDEBUG=2 -o timer.$(target)
#	$(CC) -c tsc.c -pthread -DDEBUG=2
	$(CC) -c tsc.c
	$(CC) -c interface.c
	$(CC) -c driver.c
	$(CC) $(CFLAGS) main.c tsc.o interface.o driver.o -pthread -o main.$(target)

clean:
	$(RM) *.x86 *.arm *.o
