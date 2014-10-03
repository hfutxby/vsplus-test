cmake_minimum_required(VERSION 2.8)
project(tsc)

add_definitions(-D_VSPLUS_SIM_ -D_PROZESSOR_INTEL_ -D_HELPER_MACROS_ -D_ADVANCED_MEMORY_ -DDEBUG=1)
set(SRC main.c VSP_Interface.c)
add_executable(tsc ${SRC})

set(SRC_PATH "/home/byxu/workspace/vsplus/test")
set(MYLIB "${SRC_PATH}/libvsp_core.a")
target_link_libraries(tsc ${MYLIB})
