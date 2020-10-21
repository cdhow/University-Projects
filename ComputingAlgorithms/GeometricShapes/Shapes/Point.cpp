//
// Created by caleb on 26/8/19.
//

#include "Point.h"

// #### Constructors and Assignments #### //
// Defaults
Point::Point() : x{0}, y{0}, s{'0'} {}
Point::Point(int x, int y, char s) : x{x}, y{y}, s{s} {}
// Copy Constructor
Point::Point(Point const & that) : x{that.x}, y{that.y}, s{that.s} {}
// Move Constructor
Point::Point(Point && that) noexcept : x{std::move(that.x)}, y{std::move(that.y)}, s{std::move(that.s)}
{
    that.x = 0;
    that.y = 0;
    that.s = 0;
}
// Copy Assignment
Point & Point::operator = (Point const & that)
{
    if (this != &that) {
        x = that.x;
        y = that.y;
        s = that.s;
    } return *this;
}
// Move Assignment
Point & Point::operator = (Point && that) noexcept
{
    x = std::move(that.x);
    y = std::move(that.y);
    s = std::move(that.s);
    that.x = 0;
    that.y = 0;
    that.s = 0;
}
// Destructor
Point::~Point() {}

// #### Main Methods #### //
void Point::draw(Screen & screen)
{
    screen.point(x, y, s);
}

std::string Point::info()
{
    std::string info = "Point ";
    info.append(std::to_string(x)); info.push_back(' ');
    info.append(std::to_string(y)); info.push_back(' ');
    info.push_back(s); info.push_back('\n');
    return info;
}

int Point::getX() { return x; }
int Point::getY() { return y; }
int Point::getChar() { return s; }

void Point::setX(int sx) { x = sx; }
void Point::setY(int sy) { y = sy; }
void Point::setChar(char ss) { s = ss; }