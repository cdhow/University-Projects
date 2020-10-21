//
// Created by caleb on 31/8/19.
//

#ifndef PROJECT3_1_SHAPE_H
#define PROJECT3_1_SHAPE_H

#include <iostream>
#include "../Screen.h"

class Shape {
public:
    Shape(int x, int y, char s) : x{x}, y{y}, s{s} {}
    virtual ~Shape() {}

    virtual void draw(Screen &) {}
    virtual std::string info() {}

protected:
    int x, y;
    char s;
};


#endif //PROJECT3_1_SHAPE_H
