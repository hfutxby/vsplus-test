#ifeq ($(target),arm)
#CC = arm-none-linux-gnueabi-gcc
#target = arm
#else
#CC = gcc
#target = x86
#endif

ARMCC = arm-none-linux-gnueabi-gcc
CC = gcc

#CFLAGS += -D_PROZESSOR_INTEL_ -D_HELPER_MACROS_ -D_ADVANCED_MEMORY_
CFLAGS += -D_PROZESSOR_INTEL_ -D_HELPER_MACROS_ -D_ADVANCED_MEMORY_ -DDEBUG=1
#CFLAGS += -D_PROZESSOR_INTEL_ -D_HELPER_MACROS_ -DDEBUG=3

all: main test sim_ctrl
main: main.c tsc.c VSP_Interface.c
	$(CC) $(CFLAGS) tsc.c sim.c VSP_Interface.c parse_xml.c main.c vsp_core.lib -pthread -o main.x86.out
	$(ARMCC) $(CFLAGS) tsc.c sim.c VSP_Interface.c parse_xml.c main.c vsp_core_arm.lib -pthread -o main.arm.out

test: test.c tsc.c
	$(CC) $(CFLAGS) test.c tsc.c sim.c -pthread -o test.x86.out

sim_ctrl: sim_ctrl_det.c sim_ctrl_sg.c sim_ctrl.c
	$(CC) sim_ctrl_det.c -pthread -o sim_ctrl_det.x86.out
	$(ARMCC) sim_ctrl_det.c -pthread -o sim_ctrl_det.arm.out
	$(CC) sim_ctrl_sg.c -pthread -o sim_ctrl_sg.x86.out
	$(ARMCC) sim_ctrl_sg.c -pthread -o sim_ctrl_sg.arm.out
	$(CC) sim_ctrl.c -pthread -o sim_ctrl.x86.out
	$(ARMCC) sim_ctrl.c -pthread -o sim_ctrl.arm.out

decode: decode.c
	$(CC) decode.c -o decode.out

clean:
	$(RM) *.x86 *.arm *.o *.out
