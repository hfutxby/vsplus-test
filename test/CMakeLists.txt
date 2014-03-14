cmake_minimum_required(VERSION 2.8)
project(test)

add_definitions(-D_EMULATOR_ -D_VSPLUS_SIM_ -D_PROZESSOR_INTEL_ -D_HELPER_MACROS_ -D_ADVANCED_MEMORY_)
set(SRC test.c)
add_executable(test ${SRC})

#set(VSPLIB "./vsp_core.lib")
#target_link_libraries(test ${VSPLIB})

set(MYLIB "./libmylib.a")
target_link_libraries(test ${MYLIB})
