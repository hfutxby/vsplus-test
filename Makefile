
ARMCC = /home/byxu/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-gcc
CC = gcc

CFLAGS += -O0 -D_PROZESSOR_INTEL_ -D_HELPER_MACROS_ -D_ADVANCED_MEMORY_ -DDEBUG=2  
#CFLAGS += -D_PROZESSOR_INTEL_ -D_HELPER_MACROS_ -D_ADVANCED_MEMORY_ -DDEBUG=1 -DUSE_TTY -DUSE_INI -g

all: main serial_test sg_view parse_xml test_client
main: main.c tsc.c VSP_Interface.c
	$(CC) $(CFLAGS) tsc_test_server.c VSP_Interface.c tsc.c drive.c ring_buf.c serial_pack.c prg_track.c sg_track.c vs_main.c main.c vsp_core.lib -pthread -lm -o main.x86.out
	$(ARMCC) $(CFLAGS) tsc_test_server.c VSP_Interface.c tsc.c drive.c ring_buf.c serial_pack.c prg_track.c sg_track.c vs_main.c main.c vsp_core_arm.lib -pthread -lm -o main.arm.out

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

parse_xml:
	$(CC) parse_vsplus.c `xml2-config --cflags --libs` -o parse_vsplus.x86.out

test_client:
	$(CC) tsc_test_client.c -o tsc_test_client.x86.out
	$(CC) test_set_det.c -o test_set_det.x86.out
	$(ARMCC) test_set_det.c -o test_set_det.arm.out
	$(CC) test_set_det_auto.c -o test_set_det_auto.x86.out
	$(ARMCC) test_set_det_auto.c -o test_set_det_auto.arm.out
	$(CC) test_set_pt.c -o test_set_pt.x86.out
	$(ARMCC) test_set_pt.c -o test_set_pt.arm.out

clean:
	$(RM) *.x86 *.arm *.o *.out

test: test.c
	$(CC) $(CFLAGS) test.c -o test.x86.out




