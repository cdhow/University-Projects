//
// Created by caleb on 27/8/19.
//

#include "Polygon.h"

// #### Constructors and Assignments #### //
// Defaults
Polygon::Polygon() : Point(0, 0, '0'), n{0}, l{0} {}
Polygon::Polygon(int x, int y, int n, int l, char s) : Point(x, y, s), n{n}, l{l} {}

// Copy Constructor
Polygon::Polygon(Polygon const & that) : Point(that), n{that.n}, l{that.l} {}
// Move Constructor
Polygon::Polygon(Polygon && that) noexcept : Point(std::move(that)), n{std::move(that.n)}, l{std::move(that.l)}
{
    that.n = 0; that.l = 0; that.x = 0; that.y = 0; that.s = 0;
}
// Copy Assignment
Polygon & Polygon::operator=(Polygon const & that)
{
    if (&that != this) {
        x = that.x;
        y = that.y;
        s = that.s;
        n = that.n;
        l = that.l;
    } return *this;

}
// Move Assignment
Polygon & Polygon::operator=(Polygon &&that) noexcept
{
    x = std::move(that.x);
    y = std::move(that.y);
    s = std::move(that.s);
    n = std::move(that.n);
    l = std::move(that.l);

    that.n = 0; that.l = 0; that.x = 0; that.y = 0; that.s = 0;
}
// Destructor
Polygon::~Polygon() {}

// #### Main Methods #### //
void Polygon::draw(Screen & screen)
{
    screen.polygon(x, y, n, l, s);
}

std::string Polygon::info()
{
    std::string info = "Polygon ";
    info.append(std::to_string(x)); info.push_back(' ');
    info.append(std::to_string(x)); info.push_back(' ');
    info.append(std::to_string(n)); info.push_back(' ');
    info.append(std::to_string(l)); info.push_back(' ');
    info.push_back(s); info.push_back('\n');
    return info;
}

int Polygon::getN() { return n; }
int Polygon::getL() { return l; }

void Polygon::setN(int sa) { n = sa; }
void Polygon::setL(int sb) { l = sb; }