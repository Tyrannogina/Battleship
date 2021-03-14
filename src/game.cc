
#include <iostream>

#include "game.h"

Game::Game() 
: menuSelection(0)
{
  std::cout << "LOG - Game starting\n";
}

void Game::displayMenu() {
  std::cout << "\033[1;34mWelcome to Battleship 💥 🚢 !\033[0m\n";
  std::cout << "\033[34mWhat would you like to do?\033[0m\n";
  std::cout << "\033[34m1 - One player vs. computer\033[0m\n";
  std::cout << "\033[34m0 - Quit\033[0m\n";
}

void Game::readMenuSelection() {
  std::cin >> Game::menuSelection;
}

void Game::displayWinner() {
  std::cout << "And the winner is";
}

void Game::startGame() {
  do {
    displayMenu();
    readMenuSelection();
    switch (menuSelection) {
      case 1:
        std::cout << "Game starts\n";
        break;
      case 2:
        break;
      case 0:
        std::cout << "Goodbye\n";
      default:
        std::cout << "That was not a valid menu option, please try again!\n";
    }
  } while (menuSelection != 0);
};

void endGame();
