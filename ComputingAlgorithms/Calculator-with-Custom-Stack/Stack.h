//
// Created by caleb on 14/8/19.
//

#ifndef CALCULATORV2_STACK_H
#define CALCULATORV2_STACK_H

#include <iostream>

class Stack {
private:
    int *stack;
    int itemCount;
public:
    Stack();

    // Copy constructor
    Stack(Stack const &);

    // Move constructor
    Stack(Stack &&) noexcept;

    // Copy assignment
    Stack & operator =(Stack const &);

    // Move assignment
    Stack & operator =(Stack && that) noexcept;

    // Destructor
    ~Stack();


    void push(int);

    int pop();

    int topStack();

    void printStack();
};



#endif //CALCULATORV2_STACK_H
