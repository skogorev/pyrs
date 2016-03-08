cmake_minimum_required(VERSION 2.6)
project(Pyrs C)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_definitions(
        -std=gnu99
        -msse4.1
        -pedantic
        -O3
        -Wall
        -Wextra
        -Wcast-align
        -w)

set(PYRS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(3RDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)

file(GLOB_RECURSE PYRS_SOURCES
    ${PYRS_SOURCE_DIR}/*.h
    ${PYRS_SOURCE_DIR}/*.hpp
    ${PYRS_SOURCE_DIR}/*.c
    ${PYRS_SOURCE_DIR}/*.cpp
    ${3RDPARTY_DIR}/json-parser/json.h
    ${3RDPARTY_DIR}/json-parser/json.c
    ${3RDPARTY_DIR}/vc_vector/vc_vector.h
    ${3RDPARTY_DIR}/vc_vector/vc_vector.c)
list(SORT PYRS_SOURCES)

include_directories(
        ${3RDPARTY_DIR}/include
        ${3RDPARTY_DIR}/vc_vector
        ${3RDPARTY_DIR}/json-parser)

add_executable(Pyrs ${PYRS_SOURCES})

target_link_libraries(Pyrs
        ${3RDPARTY_DIR}/lib/libhaywire.a
        ${3RDPARTY_DIR}/lib/libuv.a
        python2.7)
