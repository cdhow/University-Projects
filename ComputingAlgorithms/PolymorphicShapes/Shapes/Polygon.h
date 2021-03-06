//
// Created by caleb on 27/8/19.
//

#ifndef PROJECT3_POLYGON_H
#define PROJECT3_POLYGON_H

#include <iostream>
#include "Shape.h"

class Polygon : public Shape {
public:
    Polygon();
    Polygon(int x, int y, int n, int l, char s); 			// default constructor
    Polygon(Polygon const & that);							// copy constructor
    Polygon(Polygon && that) noexcept;						// move constructor
    Polygon & operator = (Polygon const & that); 			// copy assignment
    Polygon & operator = (Polygon && that) noexcept;		// move assignment
    ~Polygon();											    // destructor

    void draw(Screen & screen) override;
    std::string info() override;

    int getN();
    int getL();

    void setL(int sn);
    void setN(int sl);

private:
    int n, l;
};


#endif //PROJECT3_POLYGON_H
