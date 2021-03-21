
#include <iostream>

#include "lib/config_parser.h"
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

// TODO: use this for coordinate reading
std::string readLine() {
  std::string str;
  std::cin.ignore();
  std::getline(std::cin, str);
  return str;
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
      case 1: {
        std::cout << "Game starts\n";
        ConfigParser configParser;
        Config config = configParser.parseConfig();
        break;
      }
      case 2:
        break;
      case 0:
        std::cout << "Goodbye\n";
        break;
      default:
        std::cout << "That was not a valid menu option, please try again!\n";
    }
  } while (menuSelection != 0);
};

void endGame();
