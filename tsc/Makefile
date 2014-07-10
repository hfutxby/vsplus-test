#ifeq ($(target),arm)
#CC = arm-none-linux-gnueabi-gcc
#target = arm
#else
#CC = gcc
#target = x86
#endif

ARMCC = arm-none-linux-gnueabi-gcc
CC = gcc

CFLAGS += -D_PROZESSOR_INTEL_ -D_HELPER_MACROS_ -D_ADVANCED_MEMORY_ -DDEBUG=2
#CFLAGS += -D_PROZESSOR_INTEL_ -D_HELPER_MACROS_ -D_ADVANCED_MEMORY_ -DDEBUG=1

all: main serial_test sg_view decode
main: main.c tsc.c VSP_Interface.c
	$(CC) $(CFLAGS) VSP_Interface.c tsc.c drive.c ring_buf.c serial_pack.c prg_track.c sg_track.c parse_xml.c vs_main.c main.c vsp_core.lib -pthread -o main.x86.out

#用来测试发送模拟的检测器数据
serial_test: ring_buf.c serial_test.c serial_read.c
	$(CC) ring_buf.c serial_test.c -pthread -o serial_test.x86.out
	$(ARMCC) ring_buf.c serial_test.c -pthread -o serial_test.arm.out
	$(CC) ring_buf.c serial_read.c -pthread -o serial_read.x86.out

#用来观察信号灯状态
sg_view: sg_view.c
	$(CC) sg_view.c -o sg_view.x86.out

decode: base64.c
	$(CC) base64.c -o decode.out

clean:
	$(RM) *.x86 *.arm *.o *.out

test: test.c
	$(CC) $(CFLAGS) VSP_Interface.c tsc.c drive.c ring_buf.c serial_pack.c prg_track.c sg_track.c parse_xml.c vsp_core.lib -pthread test.c -o test.x86.out




