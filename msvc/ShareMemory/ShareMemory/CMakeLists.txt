cmake_minimum_required(VERSION 2.8)
set(CMAKE_BUILID_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -g gdb3 -Wno-invalid-offsetof")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -g gdb3 -O2 -Wno-invalid-offsetof")

project(ShareMemory)
set(EXECUTABLE_OUTPUT_PATH ${ShareMemory_SOURCE_DIR}/../../../build/bin/)
add_definitions(-g -D _LINUX64)
include_directories(
../../../dependency/lib_self/libNet/include
../../../frame/share_memory/com
../../../frame/share_memory/smu
../../../frame/share_memory/share_memory
../../../src/algorithm
../../../src/common
../../../src/pattern
../../../src/reflect
../../../src/util
)

link_directories(
)

file(GLOB ALL_HEADER_SRCS
../../../frame/share_memory/com/*.h
../../../frame/share_memory/smu/*.h
../../../frame/share_memory/share_memory/*.h
../../../src/common/*.h
../../../src/pattern/singleton.h
../../../src/util/*.h
)

file(GLOB ALL_CXX_SRCS
../../../frame/share_memory/com/*.cpp
../../../frame/share_memory/smu/*.cpp
../../../frame/share_memory/share_memory/*.cpp
../../../src/util/myassert.cpp
../../../src/util/helper.cpp
../../../src/util/log.cpp
../../../src/util/string_util.cpp
../../../src/util/thread.cpp
../../../src/util/time_system.cpp
../../../src/util/timer.cpp
)

set(ALL_LIB dl pthread rt)
add_executable(ShareMemory ${ALL_HEADER_SRCS} ${ALL_CXX_SRCS})
target_link_libraries(ShareMemory ${ALL_LIB})

