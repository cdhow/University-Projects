//
// Created by caleb on 27/8/19.
//

#ifndef PROJECT3_ELLIPSE_H
#define PROJECT3_ELLIPSE_H

#include <iostream>
#include "Point.h"
#include "../Screen.h"

class Ellipse : public Point {
public:
    Ellipse();
    Ellipse(int x, int y, int a, int b, char s); 			// default constructor
    Ellipse(Ellipse const & that);							// copy constructor
    Ellipse(Ellipse && that) noexcept;						// move constructor
    Ellipse & operator = (Ellipse const & that); 			// copy assignment
    Ellipse & operator = (Ellipse && that) noexcept;		// move assignment
    ~Ellipse();											    // destructor

    void draw(Screen & screen);
    std::string info();

    int getA();
    int getB();

    void setA(int sa);
    void setB(int sb);


private:
    int a, b;
};


#endif //PROJECT3_ELLIPSE_H
