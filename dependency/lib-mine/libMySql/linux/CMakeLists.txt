project(MySql)
set(LIBRARY_OUTPUT_PATH ${MySql_SOURCE_DIR}/../../../output/libMySql/linux/lib)
add_definitions(-g -D _LIB -D _LINUX64)
include_directories(
./include
./sdk/include
../../../../src/common
../../../../src/util
)

link_directories(
${MySql_SOURCE_DIR}/./sdk/lib/
)

file(GLOB ALL_HEADER_SRCS
./include/*.h
./sdk/include/*.h
../../../../src/common/type.h
../../../../src/util/assert.h
../../../../src/util/strUtil.h
)

file(GLOB ALL_CXX_SRCS
./src/*.cpp
../../../../src/util/assert.cpp
../../../../src/util/strUtil.cpp
)

set(ALL_LIB pthread dl mysqlclient)
add_library(MySql_c STATIC ${ALL_HEADER_SRCS} ${ALL_CXX_SRCS})
target_link_libraries(MySql_c ${ALL_LIB})
