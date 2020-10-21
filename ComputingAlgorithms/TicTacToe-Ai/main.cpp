#include <iostream>
#include "Game.h"

int main() {

    int run{1};

    while (run) {
        Game game;
        game.init();
        game.play();

        std:: cout << "Exit game? Press 0. Press 1 to play again:  ";
        std::cin.clear();
        std::cin >> run;

    }

    return 0;
}