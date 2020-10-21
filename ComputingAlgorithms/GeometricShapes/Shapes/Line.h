//
// Created by caleb on 27/8/19.
//

#ifndef PROJECT3_LINE_H
#define PROJECT3_LINE_H

#include <iostream>
#include "Point.h"
#include "../Screen.h"

class Line : public Point {
public:
    Line();
    Line(int x, int y, int xp, int yp, char s); 	// default constructor
    Line(Line const & that);						// copy constructor
    Line(Line && that) noexcept;					// move constructor
    Line & operator = (Line const & that); 			// copy assignment
    Line & operator = (Line && that) noexcept;		// move assignment
    ~Line();									    // destructor

    void draw(Screen & screen);
    std::string info();

    int getXP();
    int getYP();

    void setXP(int sn);
    void setYP(int sl);

private:
    int xp, yp;
};


#endif //PROJECT3_LINE_H
