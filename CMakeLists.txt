cmake_minimum_required(VERSION 3.3)

project(Tl2017)

add_subdirectory(src)

find_package(Threads REQUIRED)
if(MSVC)
    set(MSVC_COMPILER_DEFS "-D_VARIADIC_MAX=10")
endif()
set(COMMON_INCLUDES includ)
set(EXT_PROJECTS_DIR vendor)
add_subdirectory(${EXT_PROJECTS_DIR}/gtest)
add_subdirectory(test)

