//
// Created by caleb on 27/8/19.
//

#ifndef PROJECT3_LINE_H
#define PROJECT3_LINE_H

#include <iostream>
#include "Shape.h"

class Line : public Shape {
public:
    Line();
    Line(int x, int y, int xp, int yp, char s); 	// default constructor
    Line(Line const & that);						// copy constructor
    Line(Line && that) noexcept;					// move constructor
    Line & operator = (Line const & that); 			// copy assignment
    Line & operator = (Line && that) noexcept;		// move assignment
    ~Line();									    // destructor

    void draw(Screen & screen) override;
    std::string info() override;

    int getXP();
    int getYP();

    void setXP(int sn);
    void setYP(int sl);

private:
    int xp, yp;
};


#endif //PROJECT3_LINE_H
