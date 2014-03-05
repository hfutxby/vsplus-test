
target :=x86

ifeq ($(target),arm)
CC = arm-none-linux-gnueabi-gcc
else
CC = gcc
endif

test:
	$(CC) timer.c -pthread -DDEBUG=2 -o timer.$(target)
