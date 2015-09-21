cmake_minimum_required(VERSION 3.3)
project(PDS1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp my_graphs/tgr.cpp my_graphs/tgr.h my_graphs/sgr.cpp my_graphs/sgr.h)
add_executable(PDS1 ${SOURCE_FILES})