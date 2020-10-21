//
// Created by caleb on 14/8/19.
//

#include "Calc.h"

// ---------- Constructors and Assignments --------- //

Calc::Calc() : equationLength{0}, answer{0} {
    // Add dummy $ to operators
    operators.push(-1);

    // precedence[-identifier] = identifier precedence
    precedence = new int[8] {0, 0, 1, 1, 2, 2, 3, 3};
}

// Copy constructor
Calc::Calc(Calc const &that) : equationLength{that.equationLength}, answer{that.answer} {
    precedence = new int[8] {0, 0, 1, 1, 2, 2, 3, 3};

    equation = new int[equationLength];
    // Copy that.equation
    for (int i=0; i<equationLength;i++)
        equation[i] = that.equation[i];

    // Copy Stacks
    operators = that.operators;
    operands = that.operands;


}

// Move Constructor
Calc::Calc(Calc && that) noexcept : equation{std::move(that.equation)}, equationLength{std::move(that.equationLength)},
                                    answer{std::move(that.answer)}, operators{std::move(that.operators)},
                                    operands{std::move(that.operands)} {that.equation = nullptr;}

// Copy Assignment
Calc & Calc::operator =(Calc const &that) {
    if (this != &that) {
        equationLength = that.equationLength;
        equation = new int[equationLength];
        precedence = new int[8] {0, 0, 1, 1, 2, 2, 3, 3};

        // Copy that.equation array
        for (int i = 0; i < equationLength; i++)
            equation[i] = that.equation[i];

        // Copy stacks
        operators = that.operators;
        operands = that.operands;
    }
    return *this;
}

// Move Assignment
Calc & Calc::operator =(Calc && that) noexcept {
    equationLength = that.equationLength;
    equation = that.equation;
    that.equation = nullptr;
    precedence = new int[8] {0, 0, 1, 1, 2, 2, 3, 3};
    operands = that.operands;
    operators = that.operators;
}

// Destructor
Calc::~Calc() {
    delete[] equation;
    delete[] precedence;
}

// ------------ Other Methods ------------- //

void Calc::updateEquation(std::string & str) {
    // Allocates a safe amount of memory
    equation = new int[str.length()];

    // Create stream from string.
    std::stringstream iss(str);

    std::string val;

    while (iss >> val) {
        // Operand
        if (isdigit(val[0])) {
            // Push operand to equation
            equation[equationLength] = std::stoi(val);
        }
        // Operate
        else {
            int pushVal = 0;
            // Push identifier to equation
            switch (val[0])
            {
                case '(' : pushVal = -2;
                    break;
                case ')' : pushVal = -3;
                    break;
                case '+' : pushVal = -4;
                    break;
                case '-' : pushVal = -5;
                    break;
                case '*' : pushVal = -6;
                    break;
                case '/' : pushVal = -7;
                    break;
            }
            equation[equationLength] = pushVal;
        }
        equationLength += 1;
    }
}

//operator     $  (  )  +  −  ∗  /
//identifier  -1 -2 -3 -4 -5 -6 -7
//precedence 0 0  1  1  2  2  3  3
int Calc::operation(int val1, int val2, int operate) {
    switch (operate)
    {
        case -4 : return val1 + val2;
        case -5 : return val2 - val1;
        case -6 : return val1 * val2;
        case -7 : return val2 / val1;
        // No operator, return the first value popped from operand
        default: return val1;
    }
}

void Calc::calcComponent() {

    for (int i = 0; i < equationLength; i++) {
        int op = equation[i];

        // If op is operand
        if ( op >= 0) {
            operands.push(op);
        } else if (op == -2) {
            // op is (
            operators.push(-2);
        } else if (op == -3) {
            // op is )
            // Pop until operate is (
            while (true) {
                int operate = operators.pop();
                // Continue until we pop (
                if (operate == -2)
                    break;
                // Push calculation to operand stack
                int operand1 = operands.pop();
                operands.push(operation(operand1, operands.pop(), operate));
            }
        } else {
            // op is either + - * /
            // If next item on operate stack has a lower precedence then op
            // push op to stack, otherwise pop two operands and compute with popped operate
            if (precedence[-(operators.topStack())] < precedence[-op]) {
                operators.push(op);
            } else {
                // Push computed result to operand stack
                int operand1 = operands.pop();
                operands.push(operation(operand1, operands.pop(), operators.pop()));
                // Push op to operate stack
                operators.push(op);
            }
        }
        // Display stack for each iteration
        std::cout << "Operands:  ";
        operands.printStack();
        std::cout << "Operators: ";
        operators.printStack();
        std::cout << std::endl;

    }
    // Perform final calculations
    int operand1 = operands.pop();
    answer = operation(operand1, operands.pop(), operators.pop());
}

void Calc::main() {
    // Get string from user
    std::string input;
    std::getline(std::cin, input);

    // Convert string into integers equation array
    updateEquation(input);

    // Perform operation using Stacks
    calcComponent();

    std::cout << "Answer: " << answer << std::endl;

}
