cmake_minimum_required(VERSION 3.14)
project(Project3_1)

set(CMAKE_CXX_STANDARD 17)

add_executable(Project3_1 main.cpp Screen.cpp Screen.h Shapes/Point.cpp Shapes/Point.h Shapes/Ellipse.cpp
        Shapes/Ellipse.h Shapes/Polygon.cpp Shapes/Polygon.h Shapes/Line.cpp Shapes/Line.h Bunch.h Shapes/Shape.h)