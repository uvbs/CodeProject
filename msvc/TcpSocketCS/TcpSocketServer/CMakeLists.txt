cmake_minimum_required(VERSION 2.8)
set(CMAKE_BUILID_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -g gdb3 -Wno-invalid-offsetof")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -g gdb3 -O2 -Wno-invalid-offsetof")

project(TcpSocketServer)
set(EXECUTABLE_OUTPUT_PATH ${TcpSocketServer_SOURCE_DIR}/../../../build/bin/)
add_definitions(-g -D _LINUX64 -D _SERVER_)
include_directories(
../../../frame/network_frame/tcp_socket/common
../../../frame/network_frame/tcp_socket/common/packets
../../../frame/network_frame/tcp_socket/server/frame
../../../frame/network_frame/tcp_socket/server/main
../../../src/algorithm
../../../src/common
../../../src/pattern
../../../src/reflect
../../../src/util
../../../dependency/output/libNet/include
)

link_directories(
    ${TcpSocketServer_SOURCE_DIR}/../../../dependency/output/libNet/lib/
)

file(GLOB ALL_HEADER_SRCS
../../../dependency/lib_self/libNet/include/*.h
../../../frame/network_frame/tcp_socket/common/*.h
../../../frame/network_frame/tcp_socket/common/packets/*.h
../../../frame/network_frame/tcp_socket/server/frame/*.h
../../../frame/network_frame/tcp_socket/server/main/*.h
../../../src/algorithm/*.h
../../../src/common/*.h
../../../src/pattern/*.h
../../../src/reflect/*.h
../../../src/util/*.h
)

file(GLOB ALL_CXX_SRCS
../../../frame/network_frame/tcp_socket/common/*.cpp
../../../frame/network_frame/tcp_socket/common/packets/*.cpp
../../../frame/network_frame/tcp_socket/server/frame/*.cpp
../../../frame/network_frame/tcp_socket/server/main/*.cpp
../../../src/pattern/*.cpp
../../../src/reflect/*.cpp
../../../src/util/assert.cpp
../../../src/util/helper.cpp
../../../src/util/log.cpp
../../../src/util/string_util.cpp
../../../src/util/thread.cpp
../../../src/util/time_system.cpp
../../../src/util/timer.cpp
)

set(ALL_LIB dl pthread rt Net)
add_executable(TcpSocketServer ${ALL_HEADER_SRCS} ${ALL_CXX_SRCS})
target_link_libraries(TcpSocketServer ${ALL_LIB})

