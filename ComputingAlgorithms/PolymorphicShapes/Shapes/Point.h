//
// Created by caleb on 26/8/19.
//

#ifndef PROJECT3_POINT_H
#define PROJECT3_POINT_H

#include <iostream>
#include "Shape.h"

class Point : public Shape {
public:
    Point();
    Point(int x, int y, char s); 						// default constructor
    Point(Point const & that);							// copy constructor
    Point(Point && that) noexcept;						// move constructor
    Point & operator = (Point const & that); 			// copy assignment
    Point & operator = (Point && that) noexcept;		// move assignment
    ~Point();											// destructor

    void draw(Screen & screen) override;
    std::string info() override;

    int getX();
    int getY();
    int getChar();

    void setX(int sx);
    void setY(int sy);
    void setChar(char ss);

protected:
};


#endif //PROJECT3_POINT_H
