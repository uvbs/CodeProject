project(MySql)
set(LIBRARY_OUTPUT_PATH ${MySql_SOURCE_DIR}/../lib/)
add_definitions(-g -D _LIB -D _LINUX64)
include_directories(
./sdk/include
./include
../../
../../helper
)

link_directories(
${MySql_SOURCE_DIR}/./sdk/lib/
)

file(GLOB ALL_HEADER_SRCS
./include/*.h
./sdk/include/*.h
../../helper/assert.h
../../helper/strUtil.h
)

file(GLOB ALL_CXX_SRCS
./src/*.cpp
../../helper/assert.cpp
../../helper/strUtil.cpp
)

set(ALL_LIB pthread dl mysqlclient)
add_library(MySql STATIC ${ALL_HEADER_SRCS} ${ALL_CXX_SRCS})
target_link_libraries(MySql ${ALL_LIB})
