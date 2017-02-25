project(Net)
set(LIBRARY_OUTPUT_PATH ${Net_SOURCE_DIR}/../../output/libNet/lib)
add_definitions(-g -D _LIB -D _LINUX64)
include_directories(
./
./include
../../../frame/network_frame/tcp_socket/common
../../../src/common
../../../src/util
)

file(GLOB ALL_HEADER_SRCS
./include/*.h
../../../frame/network_frame/tcp_socket/common/packet.h
../../../src/common/type.h
../../../src/util/strUtil.h
../../../src/util/assert.h
)
file(GLOB ALL_CXX_SRCS
./src/*.cpp
../../../src/util/strUtil.cpp
../../../src/util/assert.cpp
)

set(ALL_LIB pthread dl)
add_library(Net STATIC ${ALL_HEADER_SRCS} ${ALL_CXX_SRCS})
target_link_libraries(Net ${ALL_LIB})
