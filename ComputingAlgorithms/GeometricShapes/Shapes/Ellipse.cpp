//
// Created by caleb on 27/8/19.
//

#include "Ellipse.h"
// #### Constructors and Assignments #### //
// Defaults
Ellipse::Ellipse() : Point(0, 0, '0'), a{0}, b{0} {}
Ellipse::Ellipse(int x, int y, int a, int b, char s) : Point(x, y, s), a{a}, b{b} {}

// Copy Constructor
Ellipse::Ellipse(Ellipse const & that) : Point(that), a{that.a}, b{that.b} {}
// Move Constructor
Ellipse::Ellipse(Ellipse && that) noexcept : Point(std::move(that)), a{std::move(that.a)}, b{std::move(that.b)}
{
    that.x = 0; that.y = 0; that.a = 0; that.b = 0; that.s = '0';
}
// Copy Assignment
Ellipse & Ellipse::operator=(Ellipse const & that)
{
    if (this != &that) {
        x = that.x;
        y = that.y;
        a = that.a;
        b = that.b;
        s = that.s;
    } return *this;
}
// Move Assignment
Ellipse & Ellipse::operator=(Ellipse &&that) noexcept
{
    x = std::move(that.x);
    y = std::move(that.y);
    a = std::move(that.a);
    b = std::move(that.b);
    s = std::move(that.s);

    that.x = 0; that.y = 0; that.a = 0; that.b = 0; that.s = 0;
}
// Destructor
Ellipse::~Ellipse() {}

// #### Main Methods #### //
void Ellipse::draw(Screen & screen)
{
    screen.ellipse(x, y, a, b, s);
}

std::string Ellipse::info()
{
    std::string info = "Ellipse ";
    info.append(std::to_string(x)); info.push_back(' ');
    info.append(std::to_string(x)); info.push_back(' ');
    info.append(std::to_string(a)); info.push_back(' ');
    info.append(std::to_string(b)); info.push_back(' ');
    info.push_back(s); info.push_back('\n');
    return info;
}

int Ellipse::getA() { return a; }
int Ellipse::getB() { return b; }

void Ellipse::setA(int sa) { a = sa; }
void Ellipse::setB(int sb) { b = sb; }
