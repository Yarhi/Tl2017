cmake_minimum_required(VERSION 3.8)
project(TPS)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        srcs/fa.c srcs/fa.h srcs/test.c)

add_executable(TPS ${SOURCE_FILES})