cmake_minimum_required(VERSION 2.8)
project(vsplus)

set(SRC main.c tsc.c interface.c sim_signal.c sim_vsplus.c)
message(STATUS "${SRC}")
add_definitions(-D_EMULATOR_ -D_VSPLUS_SIM_ -D_PROZESSOR_INTEL_ -D_HELPER_MACROS_ -D_ADVANCED_MEMORY_)
add_executable(main.x86 ${SRC})
target_link_libraries(main.x86 pthread)
