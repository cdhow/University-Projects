//
// Created by caleb on 22/07/19.
//

#include "CompAi.h"

int CompAi::checkColunms(char (&grid)[9], int (&emptyPos)[9], const char check) {
    // Check if there are any two column combination possibilities
    for (int i = 0; i <=2; i++) {
        // | O |
        // | O |
        // | - |
        if (grid[i] == check && grid[i+3] == check) {
            // Place at i + 6 if empty
            if (emptyPos[i + 6] == 0) {
                grid[i+6] = 'X';
                emptyPos[i+6] = 1;
                return 1;
            }

        }
        // Shift positions down one spot by adding 3, check these new spots
        // | - |
        // | O |
        // | O |
        if (grid[i+3] == check && grid[i+6] == check) {
            // Place at grid[i] if empty
            if (emptyPos[i] == 0) {
                grid[i] = 'X';
                emptyPos[i] = 1;
                return 1;
            }
        }
        // Check first row with last row
        // | O |
        // | - |
        // | O |
        if (grid[i] == check && grid[i+6] == check) {
            // Place at grid[i+3] if empty
            if (emptyPos[i+3] == 0) {
                grid[i+3] = 'X';
                emptyPos[i+3] = 1;
                return 1;
            }
        }
    }
    return 0;
}

int CompAi::checkRows(char (&grid)[9], int (&emptyPos)[9], const char check) {
    // Check if there are any two row combinations possibilities
    for (int j = 0; j <= 6; j+=3) {
        // O | 0 | -
        if (grid[j] == check && grid[j+1] == check) {
            // Place at j + 2 if empty
            if (emptyPos[j + 2] == 0) {
                grid[j+2] = 'X';
                emptyPos[j+2] = 1;
                return 1;
            }
        }
        // Shift positions across by adding 1
        // - | O | O
        if (grid[j+1] == check && grid[j+2] == check) {
            // Place at j if empty
            if (emptyPos[j] == 0) {
                grid[j] = 'X';
                emptyPos[j] = 1;
                return 1;
            }
        }
        // Check first column with last column
        // O | - | O
        if (grid[j] == check && grid[j+2] == check) {
            // Place at j+1 if empty
            if (emptyPos[j+1] == 0) {
                grid[j+1] = 'X';
                emptyPos[j+1] = 1;
                return 1;
            }
        }
    }
    return 0;
}

int CompAi::checkDiagnols(char (&grid)[9], int (&emptyPos)[9], const char check) {
    // Check all corners against 4 (middle) to see if there are potential diagonal wins for player or computer
    // Opposing corners are (0,8) (2,6) , if for instance 2 is found, its opposing partner (6) will be chosen

    // Check if 4 (middle) is chosen by player or computer
    if (grid[4] == check) {
        if (grid[0] == check) {
            if (emptyPos[8] == 0) {
                grid[8] = 'X';
                emptyPos[8] = 1;
                return 1;
            }
        }
        if (grid[8] == check) {
            if (emptyPos[0] == 0) {
                grid[0] = 'X';
                emptyPos[0] = 1;
                return 1;
            }

        }
        if (grid[2] == check) {
            if (emptyPos[6] == 0) {
                grid[6] = 'X';
                emptyPos[6] = 1;
                return 1;
            }

        }
        if (grid[6] == check) {
            if (emptyPos[2] == 0) {
                grid[2] = 'X';
                emptyPos[2] = 1;
                return 1;
            }
        }
    }

    // Check if opposing corners are chosen, if (2 && 6) || (0 && 8), choose 4
    if ((grid[2] == check && grid[6] == check) ||
        (grid[0] == check && grid[8] == check)) {
        if (emptyPos[4] == 0) {
            grid[4] = 'X';
            emptyPos[4] = 1;
            return 1;
        }
    }
    return 0;
}



int CompAi::compPlace(char (&grid)[9], int (&emptyPos)[9], const char check) {

    if(checkColunms(grid, emptyPos, check)) {
        return 1;
    } else if (checkRows(grid, emptyPos, check)) {
        return 1;
    } else if (checkDiagnols(grid, emptyPos, check)) {
        return 1;
    }
    return 0;
}

void CompAi::chooseRandom(char (&grid)[9], int (&emptyPos)[9]) {
    // None of the checks in compPlace() were triggered, choose random non empty spot
    int pos;
    while (true) {
        pos = rand() % 8;

        if (emptyPos[pos] == 0) {
            grid[pos] = 'X';
            emptyPos[pos] = 1;
            return;
        }
        // Else repeat until random empty spot is found
    }
}
