cmake_minimum_required(VERSION 2.8)
set(CMAKE_BUILID_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -g gdb3 -Wno-invalid-offsetof")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -g gdb3 -O2 -Wno-invalid-offsetof")
set(CMAKE_C_COMPILER "/home/chenzhifeng/bin/gcc-4.9.4/bin/gcc")
set(CMAKE_CXX_COMPILER "/home/chenzhifeng/bin/gcc-4.9.4/bin/g++")

project(CodeProj)
set(EXECUTABLE_OUTPUT_PATH ${CodeProj_SOURCE_DIR}/../../build/bin/)
add_definitions(-g -D _LINUX64)
add_definitions(-std=c++11)
include_directories(
./
../../frame/cpp_reflect
../../src/algorithm
../../src/common
../../src/container
../../src/container/dictionary
../../src/demo
../../src/filereader
../../src/filereader/ini
../../src/filereader/jconfig
../../src/filereader/json
../../src/filereader/tab
../../src/filereader/xml
../../src/json_php_serializer
../../src/json_php_serializer/json
../../src/json_php_serializer/php
../../src/main
../../src/pattern
../../src/reflect
../../src/simple_reflect
../../src/ucs
../../src/util
../../src/util/url_util
../../dependency/output/boost/include
../../dependency/output/libConhash/include
../../dependency/output/libcurl/linux/include
../../dependency/output/libJsonc/include
../../dependency/output/libMySql/linux/include
../../dependency/output/libNet/include
../../dependency/output/log4cplus/linux/include
)

link_directories(
${CodeProj_SOURCE_DIR}/../../dependency/output/boost/lib
${CodeProj_SOURCE_DIR}/../../dependency/output/libConhash/lib
${CodeProj_SOURCE_DIR}/../../dependency/output/libcurl/linux/lib
${CodeProj_SOURCE_DIR}/../../dependency/output/libevent/linux/lib
${CodeProj_SOURCE_DIR}/../../dependency/output/libJsonc/lib
${CodeProj_SOURCE_DIR}/../../dependency/output/libMySql/linux/lib
${CodeProj_SOURCE_DIR}/../../dependency/output/libNet/lib
${CodeProj_SOURCE_DIR}/../../dependency/output/log4cplus/linux/lib
)

file(GLOB ALL_HEADER_SRCS
./*.h
../../frame/cpp_reflect/*.h
../../src/algorithm/*.h
../../src/common/*.h
../../src/container/*.h
../../src/container/dictionary/*.h
../../src/demo/*.h
../../src/filereader/*.h
../../src/filereader/ini/*.h
../../src/filereader/jconfig/*.h
../../src/filereader/json/*.h
../../src/filereader/tab/*.h
../../src/filereader/xml/*.h
../../src/json_php_serializer/*.h
../../src/json_php_serializer/json/*.h
../../src/json_php_serializer/php/*.h
../../src/main/*.h
../../src/pattern/*.h
../../src/reflect/*.h
../../src/simple_reflect/*.h
../../src/ucs/*.h
../../src/util/*.h
../../src/util/url_util/*.h
../../dependency/output/boost/include/*.h
../../dependency/output/libConhash/include/*.h
../../dependency/output/libcurl/linux/include/*.h
../../dependency/output/libevent/linux/include/*.h
../../dependency/output/libJsonc/include/*.h
../../dependency/output/libMySql/linux/include/*.h
../../dependency/output/libNet/include/*.h
../../dependency/output/log4cplus/linux/include/log4cplus/*.h
)

file(GLOB ALL_CXX_SRCS
./*.cpp
../../frame/cpp_reflect/*.cpp
../../src/algorithm/*.cpp
../../src/common/*.cpp
../../src/container/*.cpp
../../src/container/dictionary/*.cpp
../../src/demo/*.cpp
../../src/filereader/*.cpp
../../src/filereader/ini/*.cpp
../../src/filereader/jconfig/*.cpp
../../src/filereader/json/*.cpp
../../src/filereader/tab/*.cpp
../../src/filereader/xml/*.cpp
../../src/json_php_serializer/*.cpp
../../src/json_php_serializer/json/*.cpp
../../src/json_php_serializer/php/*.cpp
../../src/main/*.cpp
../../src/pattern/*.cpp
../../src/reflect/*.cpp
../../src/simple_reflect/*.cpp
../../src/ucs/*.cpp
../../src/util/*.cpp
../../src/util/url_util/*.cpp
)

set(ALL_LIB dl pthread curl ssl crypto Conhash Jsonc MySql_c mysqlclient Net
    rt log4cplus boost_date_time boost_filesystem  boost_system boost_thread
    libboost_regex.a libevent.a)
add_executable(CodeProj ${ALL_HEADER_SRCS} ${ALL_CXX_SRCS})
target_link_libraries(CodeProj ${ALL_LIB})

