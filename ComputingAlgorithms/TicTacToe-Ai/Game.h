//
// Created by caleb on 22/07/19.
//

#ifndef UNBEATABLETICTACTOE_GAME_H
#define UNBEATABLETICTACTOE_GAME_H

#include <iostream>
#include "CompAi.h"

class Game: public CompAi  {
protected:
    char grid[9];
    int emptyPos[9];
    int emptyCount;

    void displayBoard();

    void compInput();

    void playerInput();

    // Returns 0 for no win, 1 for player, 2 for comp
    int checkWin();

public:

    void init();
    void play();
};



#endif //UNBEATABLETICTACTOE_GAME_H
