//
// Created by caleb on 14/8/19.
//

#include "Stack.h"

// ---------- Constructors and Assignments ------- //

Stack::Stack() : itemCount{0} {
    stack = new int[256];
}

// Copy constructor
Stack::Stack(Stack const &that) : itemCount{that.itemCount} {
    stack = new int[itemCount];
    // Copy that.stack array
    for (int i = 0; i < itemCount; i++)
        stack[i] = that.stack[i];

}

// Move Constructor
Stack::Stack(Stack && that) noexcept : itemCount{std::move(that.itemCount)}, stack{std::move(that.stack)} {
    that.stack = nullptr;
}

// Copy Assignment
Stack & Stack::operator =(Stack const &that) {
    if (this != &that) {
        itemCount = that.itemCount;
        stack = new int[itemCount];

        // Copy that.stack array
        for (int i = 0; i < itemCount; i++)
            stack[i] = that.stack[i];
    }
    return *this;
}

// Move Assignment
Stack & Stack::operator =(Stack && that) noexcept {
    itemCount = that.itemCount;
    stack = that.stack;
    that.stack = nullptr;
}

// Destructor
Stack::~Stack() {
    delete[] stack;
}

// ------------ Other Methods ------------- //

void Stack::push(int value) {
    itemCount += 1;
    stack[itemCount-1] = value;
}

int Stack::pop() {
    if (itemCount <= 0)
        return 0;
    // itemCount - 1 will shift push position to overwrite popped int
    itemCount -= 1;
    return stack[itemCount];
}

int Stack::topStack() {
    return stack[itemCount-1];
}

void Stack::printStack() {
    for (int i = 0; i < itemCount; i++)
        std::cout << stack[i] << " ";
    std::cout << std::endl;
}