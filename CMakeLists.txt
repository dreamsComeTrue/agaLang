cmake_minimum_required(VERSION 2.6)
SET(CMAKE_BUILD_TYPE Debug)
project(agalang)

FILE(GLOB_RECURSE SRC_ALL "src/*.cpp")
FILE(GLOB_RECURSE INC_ALL "include/*.h")
FILE(GLOB_RECURSE TEST_ALL "test/*.*")
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(include)
include_directories(include)

add_executable(agalang ${SRC_ALL} ${INC_ALL} ${TEST_ALL})

install(TARGETS agalang RUNTIME DESTINATION bin)

ADD_DEFINITIONS(-std=c++0x)
