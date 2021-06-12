cmake_minimum_required(VERSION 3.5)

project(notSofiaRtspServer LANGUAGES CXX C)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(notSofiaLib)
add_subdirectory(3rdparty/ZLMediaKit)

include_directories(3rdparty/ZLMediaKit/src/)
include_directories(3rdparty/ZLMediaKit/3rdpart/ZLToolKit/src/)

file(GLOB MediaServer_src_list src/*.cpp src/*.h)
add_executable(${PROJECT_NAME} ${MediaServer_src_list})

target_link_libraries(${PROJECT_NAME} notSofiaLib zlmediakit zltoolkit pthread)

set_elf_rpath_and_ld_path(${PROJECT_NAME})
