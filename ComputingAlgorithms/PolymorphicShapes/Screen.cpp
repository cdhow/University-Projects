//
// Created by caleb on 26/8/19.
//

#include "Screen.h"

// default constructor
Screen::Screen() : mWidth(ratio * dim * 2 + 1), mHeight(dim * 2 + 1)
{
    mBoard = new char * [mHeight];
    for(int j = 0; j < mHeight; ++j)
        mBoard[j] = new char [mWidth]();
}

// destructor
Screen::~Screen()
{
    for(int j = 0; j < mHeight; ++j)
        delete [] mBoard[j];
    delete mBoard;
}

// copy constructor
Screen::Screen(Screen const & that) : mWidth(that.mWidth), mHeight(that.mHeight)
{
    mBoard = new char * [mHeight];
    for(int j = 0; j < mHeight; ++j)
    {
        mBoard[j] = new char [mWidth];
        for (int i = 0; i < mWidth; ++i)
            mBoard[j][i] = that.mBoard[j][i];
    }
}

// move constructor
Screen::Screen(Screen && that) noexcept :
        mWidth(std::move(that.mWidth)), mHeight(std::move(that.mHeight)), mBoard(std::move(that.mBoard))
{
    that.mBoard = nullptr;
    that.mWidth = 0;
    that.mHeight = 0;
}

// copy assignment
Screen & Screen::operator = (Screen const & that)
{
    if (this != &that) return *this;

    if (mHeight != that.mHeight || mWidth != that.mWidth)
    {
        for(int j = 0; j < mHeight; ++j)
            delete [] mBoard[j];
        delete mBoard;
        mWidth = that.mWidth;
        mHeight = that.mHeight;
        mBoard = new char * [mHeight];
        for(int j = 0; j < mHeight; ++j)
            mBoard[j] = new char [mWidth]();
    }

    for(int j = 0; j < mHeight; ++j)
        for (int i = 0; i < mWidth; ++i)
            mBoard[j][i] = that.mBoard[j][i];

    return *this;
}

// move constructor
Screen & Screen::operator = (Screen && that) noexcept
{
    mWidth = std::move(that.mWidth);
    mHeight = std::move(that.mHeight);
    mBoard = std::move(that.mBoard);

    that.mBoard = nullptr;
    that.mWidth = 0;
    that.mHeight = 0;

    return *this;
}

// plot a point
void Screen::plot(double x, double y, char s)
{
    int yy = static_cast<int>(round(-y + mHeight / 2) );
    int xx = static_cast<int>(round(ratio * x + mWidth / 2 + ratio / 2 ));
    mBoard[yy][xx] = s;
}

// getter for dmax
int Screen::dmax() const
{
    return dim;
}

// getter for dmin
int Screen::dmin() const
{
    return - dim;
}

// display screen
void Screen::display() const
{
    for(int j = 0; j < mHeight; ++j)
    {
        for(int i = 0; i < mWidth; ++i)
            if (mBoard[j][i])
                std::cout << mBoard[j][i];
            else
                std::cout << ' ';
        std::cout << std::endl;
    }
}

// clear the screen
void Screen::clear()
{
    for(int j = 0; j < mHeight; ++j)
        for(int i = 0; i < mWidth; ++i)
            mBoard[j][i] = 0;
}

// show the axes using symbol s
void Screen::axes(char s)
{
    // plot x axis
    for(int x = dmin(); x <= dmax(); ++x)
        plot(x,0,s);
    // plot y axis
    for(int y = dmin(); y <= dmax(); ++y)
        plot(0,y,s);
}

// plot a point at (x,y) with symbol s
void Screen::point(int x, int y, char s)
{
    plot(static_cast<double>(x), static_cast<double>(y), s);
}

// plot an ellipse using center (xc, yc), radii rx and ry, and symbol s
void Screen::ellipse(int xc, int yc, int rx, int ry, char s)
{
    double dx, dy, d1, d2, x, y;
    x = 0;
    y = ry;

    // initial decision parameter of region 1
    d1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);
    dx = 2 * ry * ry * x;
    dy = 2 * rx * rx * y;

    // for region 1
    while (dx < dy)
    {
        // print points based on 4-way symmetry
        plot(x + xc, y + yc, s);
        plot(-x + xc, y + yc, s);
        plot(x + xc, -y + yc, s);
        plot(-x + xc, -y + yc, s);

        // checking and updating value of
        // decision parameter based on algorithm
        if (d1 < 0)
        {
            x++;
            dx = dx + (2 * ry * ry);
            d1 = d1 + dx + (ry * ry);
        }
        else
        {
            x++;
            y--;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d1 = d1 + dx - dy + (ry * ry);
        }
    }

    // decision parameter of region 2
    d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) +
         ((rx * rx) * ((y - 1) * (y - 1))) -
         (rx * rx * ry * ry);

    // plotting points of region 2
    while (y >= 0)
    {
        // print points based on 4-way symmetry
        plot(x + xc, y + yc, s);
        plot(-x + xc, y + yc, s);
        plot(x + xc, -y + yc, s);
        plot(-x + xc, -y + yc, s);

        // checking and updating parameter
        // value based on algorithm
        if (d2 > 0)
        {
            y--;
            dy = dy - (2 * rx * rx);
            d2 = d2 + (rx * rx) - dy;
        }
        else
        {
            y--;
            x++;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d2 = d2 + dx - dy + (rx * rx);
        }
    }
}

// plot a regular polygon with center (x,y), length l, n sides, using symbol s
void Screen::polygon(int x, int y, int n, int l, char s)
{
    double * xx = new double [n];
    double * yy = new double [n];
    for(int k = 0; k < n; ++k)
    {
        xx[k] = l * cos(2*pi*k/n);
        yy[k] = l * sin(2*pi*k/n);
    }
    for(int k = 0; k < n; ++k)
        line(x + xx[k], y + yy[k], x + xx[(k+1) % n], y + yy[(k+1) % n],s);
}

// plot a line from (x0,y0) to (x1, y1) using symbol s
void Screen::line(int x0, int y0, int x1, int y1, char s)
{
    if (x1 == x0)
    {
        if (y1 > y0)
            for(double y = y0; y <= y1; y++)
                plot(x0, y, s);
        else
            for(double y = y1; y <= y0; y++)
                plot(x0, y, s);
        return;
    }

    if (y1 == y0)
    {
        if (x1 > x0)
            for(double x = x0; x <= x1; x++)
                plot(x, y0, s);
        else
            for(double x = x1; x <= x0; x++)
                plot(x, y0, s);
        return;
    }

    double m = 1.0 * (y1 - y0) / (x1 - x0);
    double c = 1.0 * (y0 * x1 - y1 * x0) / (x1 - x0);

    if (abs(x1-x0) > abs(y1 - y0))
    {
        if (x1 > x0)
            for(double x = x0; x <= x1; x+=1)
                plot(x, m * x + c, s);
        else
            for(double x = x1; x <= x0; x+=1)
                plot(x, m * x + c, s);
    }
    else
    {
        if (y1 > y0)
            for(double y = y0; y <= y1; y+=1)
                plot((y - c)/m, y, s);
        else
            for(double y = y1; y <= y0; y+=1)
                plot((y - c)/m, y, s);
    }
}