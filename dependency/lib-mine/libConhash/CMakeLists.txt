project(Conhash)
set(LIBRARY_OUTPUT_PATH ${Conhash_SOURCE_DIR}/../../output/libConhash/lib)
add_definitions(-g -D _LIB -D _LINUX64)
include_directories(
./
./include
)

file(GLOB ALL_HEADER_SRCS
./include/*.h
)
file(GLOB ALL_CXX_SRCS
./src/*.c
)

#set(ALL_LIB pthread dl)
add_library(Conhash STATIC ${ALL_HEADER_SRCS} ${ALL_CXX_SRCS})
target_link_libraries(Conhash ${ALL_LIB})

