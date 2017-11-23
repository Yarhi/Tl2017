cmake_minimum_required(VERSION 3.3)

project(Tl2017_src)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        fa.c test.c fa.h)

add_executable(Tl2017 ${SOURCE_FILES})
