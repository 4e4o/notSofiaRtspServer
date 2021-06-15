cmake_minimum_required(VERSION 3.5)

project(notSofiaRtspServer LANGUAGES CXX C)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ZLMEDIA_KIT_DIR "3rdparty/ZLMediaKit")

add_subdirectory(notSofiaLib)
add_subdirectory("${ZLMEDIA_KIT_DIR}")

file(GLOB SRC_LIST src/*.cpp src/*.h)
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    "${ZLMEDIA_KIT_DIR}/src/"
    "${ZLMEDIA_KIT_DIR}/3rdpart/ZLToolKit/src/")

target_link_libraries(${PROJECT_NAME} notSofiaLib zlmediakit zltoolkit pthread)

set_elf_rpath_and_ld_path(${PROJECT_NAME})
