cmake_minimum_required(VERSION 3.15)
project(MaxScore)

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_SOURCE_DIR src)

add_executable(MaxScore main.cpp PriorityQueue.cpp PriorityQueue.h PriorityQueueMaxDigits.cpp PriorityQueueMaxDigits.h)