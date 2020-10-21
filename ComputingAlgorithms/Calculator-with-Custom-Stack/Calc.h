//
// Created by caleb on 14/8/19.
//

#ifndef CALCULATORV2_CALC_H
#define CALCULATORV2_CALC_H

#include <string>
#include <sstream>
#include <iostream>

#include "Stack.h"

class Calc {
private:
    const int *precedence;
    int *equation;
    int equationLength;
    int answer;

    Stack operators;
    Stack operands;
public:
    Calc();

    // Copy constructor
    Calc(Calc const &);

    // Move constructor
    Calc(Calc &&) noexcept;

    // Copy assignment
    Calc & operator =(Calc const &);

    // Move assignment
    Calc & operator =(Calc && that) noexcept;

    // Destructor
    ~Calc();

    // Converts string into integer array expression
    void updateEquation(std::string &);

    // Calculates two ints with given operate
    int operation(int, int, int);

    // Takes an int and performs Stack operations
    void calcComponent();

    // Main functionality
    void main();


};


#endif //CALCULATORV2_CALC_H
