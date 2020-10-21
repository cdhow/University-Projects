//
// Created by caleb on 22/07/19.
//

#ifndef UNBEATABLETICTACTOE_COMPAI_H
#define UNBEATABLETICTACTOE_COMPAI_H

#include <iostream>

// Algorithm
// Check columns // check empty, if not empty skip placement

// Check rows   // check empty, if not empty skip placement

// Check diagonals grid[4] against corners // found // check if opposing corner is empty, if not empty skip placement

// All above fails // Choose random spot that is not empty

class CompAi {
protected:
    // All functions are passed the char check parameter, the Ai first checks if it can place for a win
    // then checks if it can place for a save (check = 'X' then 'O')
    // All three functions return 1 if placement made or 0 if not
    int checkColunms(char(&)[9], int(&)[9], char);
    int checkRows(char(&)[9], int(&)[9], char);
    int checkDiagnols(char(&)[9], int(&)[9], char);

    // Calls all check functions, returns 1 if X place or zero if not placed
    int compPlace(char(&)[9], int(&)[9], char);

    // All other checks were not triggered, choose random non empty spot
    void chooseRandom(char(&)[9], int(&)[9]);

};



#endif //UNBEATABLETICTACTOE_COMPAI_H
