cmake_minimum_required(VERSION 3.15)
project(MaxScore2)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O3")

add_executable(MaxScore2 main.cpp PriorityQueue.cpp PriorityQueue.h)