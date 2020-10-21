//
// Created by caleb on 27/8/19.
//

#ifndef PROJECT3_BUNCH_H
#define PROJECT3_BUNCH_H


#include <iostream>
#include "Shapes/Ellipse.h"
#include "Shapes/Line.h"
#include "Shapes/Point.h"
#include "Shapes/Polygon.h"

template<typename T>
class Bunch {
private:
    T *bunch;
    int itemCount;
public:
    Bunch() : itemCount{0}
    {
        bunch = new T[32];
    }
    // Copy constructor
    Bunch(Bunch const & that) : itemCount{that.itemCount} {
        bunch = new T[32];
        // Copy that.bunch array
        for (int i = 0; i < itemCount; i++)
            bunch[i] = that.bunch[i];
    }
    // Move constructor
    Bunch(Bunch && that) noexcept : itemCount{std::move(that.itemCount)}, bunch{std::move(that.bunch)}
    {
        that.bunch = nullptr;
    }
    // Copy assignment
    Bunch & operator =(Bunch const & that)
    {
        if (this != &that) {
            itemCount = that.itemCount;
            bunch = new T[32];

            // Copy that.bunch array
            for (int i = 0; i < itemCount; i++)
                bunch[i] = that.bunch[i];
        }
        return *this;
    }
    // Move assignment
    Bunch & operator =(Bunch && that) noexcept
    {
        itemCount = that.itemCount;
        bunch = that.bunch;
        that.bunch = nullptr;
    }
    // Destructor
    ~Bunch() {}

    // ------------ Other Methods ------------- //

    void push(T val)
    {
        bunch[itemCount] = val;
        itemCount++;
    }

    T pop()
    {
        itemCount--;
        return bunch[itemCount];
    }

    void clear_bunch()
    {
        delete[] bunch;
        bunch = new T[32];
        itemCount = 0;
    }

    T topBunch() { return bunch[itemCount - 1]; }

    void printBunch()
    {
        for (int i=0; i<itemCount; i++)
            std::cout << bunch[itemCount] << "\n";
        std::cout << std::endl;
    }

    int get_itemCount() { return itemCount; }

    T & operator[](int i) { return bunch[i]; }
};



#endif //PROJECT3_BUNCH_H
