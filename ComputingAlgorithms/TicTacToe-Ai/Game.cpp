//
// Created by caleb on 22/07/19.
//

#include "Game.h"

void Game::init() {
    // Gen random seed
    std::srand(time(nullptr));
    // 0 out positions
    for (int i = 0; i < 9; i++) {
        grid[i] = '-';
        emptyPos[i] = 0;
    }
    emptyCount = 8;
}

void Game::displayBoard() {
    std::cout << std::endl;
    std::cout << "   |   |   "<< std::endl;
    std::cout << " "<< grid[0] <<" | "<< grid[1] <<" | "<< grid[2] << std::endl;
    std::cout << "   |   |   "<< std::endl;
    std::cout << "-----------"<< std::endl;
    std::cout << "   |   |   "<< std::endl;
    std::cout << " "<< grid[3] <<" | "<< grid[4] << " | "<< grid[5] << std::endl;
    std::cout << "   |   |   "<< std::endl;
    std::cout << "-----------"<< std::endl;
    std::cout << "   |   |   "<< std::endl;
    std::cout << " "<< grid[6] <<" | "<< grid[7]<< " | "<< grid[8] << std::endl;
    std::cout << "   |   |   "<< std::endl;
    std::cout << std::endl;
}

void Game::compInput() {

    if (emptyCount < 0)
        return;

    // First call to see if computer has a possible win with 'X'
    if (CompAi::compPlace(grid, emptyPos, 'X')) {
        emptyCount -= 1;
        return;
        // Win not found so check for save
    } else if (CompAi:: compPlace(grid, emptyPos, 'O')) {
        emptyCount -= 1;
        return;
    }

    // Win or save not triggered so choose random non empty spot
    CompAi::chooseRandom(grid, emptyPos);

    emptyCount -= 1;


}


void Game::playerInput() {
    int pos;
    while (true) {
        std::cout << "Enter the desired location: " << std::endl;
        std::cin >> pos;
        // Check if input was valid
        if (std::cin.fail() || pos < 1 || pos > 9) {
            // User didn't input a number
            std::cin.clear();   // reset failbit
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip bad input
            std::cout << "Invalid number, choose between 1-9" << std::endl;
        } else {
            pos -=1;
            if (emptyPos[pos] == 1) {
                std::cout << "Location is not empty, try again." << std::endl;
                std::cin.clear();
            } else {
                grid[pos] = 'O';
                emptyPos[pos] = 1;
                emptyCount -= 1;
                break;
            }
        }
    }
}


int Game::checkWin() {
    char check;

    // Loop twice to check for X then O
    for (int i = 0; i < 2; i++) {

        if (i == 0)
            check = 'O';    // check for user
        else
            check = 'X';    // check for computer

        // Check rows
        for (int j = 0; j <= 6; j += 3) {
            if (grid[j] == check
                && grid[j + 1] == check
                && grid[j + 2] == check)
                return (i+1);
        }

        // Check columns
        for (int k = 0; k < 3; k++) {
            if (grid[k] == check
                && grid[k + 3] == check
                && grid[k + 6] == check)
                return (i+1);
        }

        // Check diagonals
        if (grid[0] == check
            && grid[4] == check
            && grid[8] == check)
            return (i+1);

        if (grid[2] == check
            && grid[4] == check
            && grid[6] == check)
            return (i+1);
    }
    return 0;
}



void Game::play() {
    std::cout << "\nWelcome to Tic Tac Toe." << std::endl;
    int count{0};
    while (true) {
        displayBoard();
        if (checkWin() == 1) {
            std::cout << "Player Wins.\n";
            break;
        } else if (checkWin() == 2) {
            std::cout << "Computer Wins.\n";
            break;
        } else {
            // Check if any positions left on board
            if (emptyCount <= 0) {
                std::cout << "Tie Game.\n";
                break;
            }
            // MAIN GAME CODE
            // Alternate inputs
            if (count % 2 == 0)
                playerInput();
            else
                compInput();
        }
        count++;
    }
}
