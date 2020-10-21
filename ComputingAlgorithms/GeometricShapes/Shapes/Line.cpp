//
// Created by caleb on 27/8/19.
//

#include "Line.h"

// #### Constructors and Assignments #### //
// Defaults
Line::Line() : Point(0, 0, '0'), xp{0}, yp{0} {}
Line::Line(int x, int y, int xp, int yp, char s) : Point(x, y, s), xp{xp}, yp{yp}{}

// Copy Constructor
Line::Line(Line const & that) : Point(that), xp{that.xp}, yp{that.yp} {}
// Move Constructor
Line::Line(Line && that) noexcept : Point(std::move(that)), xp{std::move(that.xp)}, yp{std::move(that.yp)}
{
    that.x = 0; that.y = 0; that.xp = 0; that.yp = 0; that.s = '0';
}
// Copy Assignment
Line & Line::operator=(Line const & that)
{
    if (this != &that) {
        x = that.x;
        y = that.y;
        xp = that.xp;
        yp = that.yp;
        s = that.s;
    } return *this;
}
// Move Assignment
Line & Line::operator=(Line &&that) noexcept
{
    x = std::move(that.x);
    y = std::move(that.y);
    xp = std::move(that.xp);
    yp = std::move(that.yp);
    s = std::move(that.s);

    that.x = 0; that.y = 0; that.xp = 0; that.yp = 0; that.s = 0;
}
// Destructor
Line::~Line() {}

// #### Main Methods #### //
void Line::draw(Screen & screen)
{
    screen.line(x, y, xp, yp, s);
}

std::string Line::info()
{
    std::string info = "Line ";
    info.append(std::to_string(x)); info.push_back(' ');
    info.append(std::to_string(x)); info.push_back(' ');
    info.append(std::to_string(xp)); info.push_back(' ');
    info.append(std::to_string(yp)); info.push_back(' ');
    info.push_back(s); info.push_back('\n');
    return info;
}

int Line::getXP() { return xp; }
int Line::getYP() { return yp; }

void Line::setXP(int sxp) { xp = sxp; }
void Line::setYP(int syp) { yp = syp; }