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
#CFLAGS += -D_PROZESSOR_INTEL_ -D_HELPER_MACROS_ -D_ADVANCED_MEMORY_ -DDEBUG=1 -DUSE_TTY -DUSE_INI -g

all: main serial_test sg_view det_ctrl decode
main: main.c tsc.c VSP_Interface.c
	$(CC) $(CFLAGS) VSP_Interface.c tsc.c drive.c ring_buf.c serial_pack.c prg_track.c sg_track.c parse_xml.c det_read.c vs_main.c main.c vsp_core.lib -pthread -o main.x86.out
	$(ARMCC) $(CFLAGS) VSP_Interface.c tsc.c drive.c ring_buf.c serial_pack.c prg_track.c sg_track.c parse_xml.c det_read.c vs_main.c main.c vsp_core_arm.lib -pthread -o main.arm.out

#用来测试发送模拟的检测器数据
serial_test: ring_buf.c serial_test.c serial_read.c
	$(CC) ring_buf.c serial_test.c -pthread -o serial_test.x86.out
	$(ARMCC) ring_buf.c serial_test.c -pthread -o serial_test.arm.out
	$(CC) ring_buf.c serial_read.c -pthread -o serial_read.x86.out
	$(ARMCC) ring_buf.c serial_read.c -pthread -o serial_read.arm.out

#用来观察信号灯状态
sg_view: sg_view.c
	$(CC) sg_view.c -o sg_view.x86.out
	$(ARMCC) sg_view.c -o sg_view.arm.out

#用来产生本地检测器数据
det_ctrl: det_ctrl.c
	$(CC) det_ctrl.c -o det_ctrl.x86.out
	$(ARMCC) det_ctrl.c -o det_ctrl.arm.out

decode: base64.c
	$(CC) base64.c -o decode.out

clean:
	$(RM) *.x86 *.arm *.o *.out

test: test.c
	$(CC) $(CFLAGS) test.c ring_buf.c -o test.x86.out




