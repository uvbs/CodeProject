cmake_minimum_required(VERSION 2.8)
set(CMAKE_BUILID_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -g gdb3 -Wno-invalid-offsetof")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -g gdb3 -O2 -Wno-invalid-offsetof")

project(HttpServer)
set(EXECUTABLE_OUTPUT_PATH ${HttpServer_SOURCE_DIR}/../../build/bin/)
add_definitions(-g -D _LINUX64)
include_directories(
../../frame/http_service
../../src/algorithm
../../src/common
../../src/container
../../src/pattern
../../src/util
../../dependency/output/libNet/include
../../dependency/output/libevent/linux/include
)

link_directories(
${HttpServer_SOURCE_DIR}/../../dependency/output/libevent/linux/lib
)

file(GLOB ALL_HEADER_SRCS
./*.h
../../frame/http_service/*.h
../../src/algorithm/mcAlloc.h
../../src/common/type.h
../../src/container/mchash.h
../../src/pattern/singleton.h
../../src/util/assert.h
../../src/util/helper.h
../../src/util/log.h
../../src/util/string_util.h
../../src/util/thread.h
../../src/util/time_system.h
../../dependency/output/libNet/include/*.h
../../dependency/output/libevent/linux/include/*.h
)

file(GLOB ALL_CXX_SRCS
./*.cpp
../../frame/http_service/*.cpp
../../src/util/assert.cpp
../../src/util/helper.cpp
../../src/util/log.cpp
../../src/util/string_util.cpp
../../src/util/thread.cpp
../../src/util/time_system.cpp
)

set(ALL_LIB dl pthread libevent.a rt)
add_executable(HttpServer ${ALL_HEADER_SRCS} ${ALL_CXX_SRCS})
target_link_libraries(HttpServer ${ALL_LIB})

