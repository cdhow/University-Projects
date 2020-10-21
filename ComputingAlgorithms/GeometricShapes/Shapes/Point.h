//
// Created by caleb on 26/8/19.
//

#ifndef PROJECT3_POINT_H
#define PROJECT3_POINT_H

#include <iostream>
#include "../Screen.h"

class Point {
public:
    Point();
    Point(int x, int y, char s); 						// default constructor
    Point(Point const & that);							// copy constructor
    Point(Point && that) noexcept;						// move constructor
    Point & operator = (Point const & that); 			// copy assignment
    Point & operator = (Point && that) noexcept;		// move assignment
    ~Point();											// destructor

    virtual void draw(Screen & screen);
    std::string info();

    int getX();
    int getY();
    int getChar();

    virtual void setX(int sx);
    virtual void setY(int sy);
    virtual void setChar(char ss);

protected:
    int x, y;
    char s;
};


#endif //PROJECT3_POINT_H
