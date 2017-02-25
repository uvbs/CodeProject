cmake_minimum_required(VERSION 2.8)
set(CMAKE_BUILID_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -g gdb3 -Wno-invalid-offsetof")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -g gdb3 -O2 -Wno-invalid-offsetof")

project(TcpServer)
set(EXECUTABLE_OUTPUT_PATH ${TcpServer_SOURCE_DIR}/../../build/bin/)
add_definitions(-g -D _LINUX64)
include_directories(
../../frame/tcp_service
../../frame/tcp_service/frame
../../frame/tcp_service/test
../../src/common
../../src/util
../../dependency/output/libevent/linux/include/
)

link_directories(
${TcpServer_SOURCE_DIR}/../../dependency/output/libevent/linux/lib
${TcpServer_SOURCE_DIR}/../../dependency/output/openssl/lib
)

file(GLOB ALL_HEADER_SRCS
./*.h
../../frame/tcp_service/*.h
../../frame/tcp_service/frame/*.h
../../frame/tcp_service/test/*.h
../../src/common/*.h
../../src/util/string_util.h
../../src/util/assert.h
../../src/util/thread.h
../../dependency/output/libevent/linux/include/*.h
)

file(GLOB ALL_CXX_SRCS
./*.cpp
../../frame/tcp_service/*.cpp
../../frame/tcp_service/frame/*.cpp
../../frame/tcp_service/test/*.cpp
../../src/util/string_util.cpp
../../src/util/assert.cpp
../../src/util/thread.cpp
)

set(ALL_LIB dl pthread libevent.a libevent_core.a libevent_extra.a
	libevent_openssl.a libevent_pthreads.a libssl.a libcrypto.a)
add_executable(TcpServer ${ALL_HEADER_SRCS} ${ALL_CXX_SRCS})
target_link_libraries(TcpServer ${ALL_LIB})
