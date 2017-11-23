cmake_minimum_required(VERSION 3.3)

project(Tl2017)

add_subdirectory(src)

find_package(Threads REQUIRED)
if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-Wall -ansi -Wno-deprecated -pthread)
endif()

if(MSVC)
    set(MSVC_COMPILER_DEFS "-D_VARIADIC_MAX=10")
endif()
set(COMMON_INCLUDES include)
set(EXT_PROJECTS_DIR vendor)
add_subdirectory(${EXT_PROJECTS_DIR}/gtest)
add_subdirectory(test)

