cmake_minimum_required(VERSION 2.6)
project(Pyrs C)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_definitions(-std=gnu99)
add_definitions(-msse4.1)
add_definitions(-pedantic)
add_definitions(-O3)
add_definitions(-Wall)
add_definitions(-Wextra)
add_definitions(-Wcast-align)
add_definitions(-w)

if (UNIX)
    add_definitions(-DUNIX)
endif (UNIX)

file(GLOB_RECURSE HAYWIRE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/haywire/src/haywire/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/haywire/src/haywire/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/haywire/src/haywire/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/haywire/src/haywire/*.cpp)

list(SORT HAYWIRE_SOURCES)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/haywire/lib/libuv/include)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/haywire/include)

find_package(Threads REQUIRED)

add_library(haywire STATIC ${HAYWIRE_SOURCES})

target_link_libraries (haywire ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_SOURCE_DIR}/haywire/lib/libuv/.libs/libuv.a)

GET_PROPERTY(haywire_location TARGET haywire PROPERTY LOCATION)

#-----------------------------------------------------------------------------------------------------

# PYRS

file(GLOB_RECURSE PYRS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/json-parser/json.h
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/json-parser/json.c
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vc_vector/vc_vector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vc_vector/vc_vector.c)

include_directories(
        ${CMAKE_SOURCE_DIR}/3rdparty/vc_vector
        ${CMAKE_SOURCE_DIR}/3rdparty/json-parser)

list(SORT PYRS_SOURCES)
find_package(Threads REQUIRED)

add_executable(Pyrs ${PYRS_SOURCES})

add_dependencies(Pyrs
        haywire
        vc_vector
        json_parser)

target_link_libraries(Pyrs
        ${haywire_location}
        ${vc_vector}
        ${CMAKE_SOURCE_DIR}/3rdparty/haywire/lib/libuv/.libs/libuv.a
        ${CMAKE_THREAD_LIBS_INIT}
        python2.7)
