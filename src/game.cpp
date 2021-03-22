
#include <iostream>
#include <string>

#include "lib/config_parser.h"
#include "game.h"

/// Constructor calls the config parser to initialise the game and creates the boards accordingly
Game::Game() {
  ConfigParser configParser;
  config = configParser.parseConfig();
  // TODO: figure out how to combine this in one method.
  createPlayer1Board();
  createPlayer2Board();
}

/// Destructor destroys the boards
Game::~Game() {
  delete player1Board;
  delete player2Board;
}

/**
  * Displays initial menu with game options and option for quitting.
  */
void Game::displayMenu() {
  std::cout << "\033[1;34mWelcome to Battleship 💥 🚢 !\033[0m\n";
  std::cout << "\033[34mWhat would you like to do?\033[0m\n";
  std::cout << "\033[34m1 - One player vs. computer\033[0m\n";
  std::cout << "\033[34m0 - Quit\033[0m\n";
}

/**
  * Reads a line inputted by the user on the terminal.
  */
std::string readLine() {
  std::string str;
  std::getline(std::cin, str);
  return str;
}

void Game::displayShipPlacementMenu(Ship currentShip) {
  
  std::cout << "\033[34mWrite the starting coordinate for your ship\033[0m\n";
  std::cout << "\033[34m0 - Quit\033[0m\n";
}

void Game::createPlayer1Board() {
  player1Board = new Cell*[config.board.height];

  for (int i = 0; i < config.board.height; i++) {
    player1Board[i] = new Cell[config.board.width];
    for (int j = 0; j < config.board.width; j++) {
      Cell emptyCell = {
        'W',
        false
      };
      player1Board[i][j] = emptyCell;
    }
  }
}

void Game::createPlayer2Board() {
  player2Board = new Cell*[config.board.height];

  for (int i = 0; i < config.board.height; i++) {
    player2Board[i] = new Cell[config.board.width];
    for (int j = 0; j < config.board.width; j++) {
      Cell emptyCell = {
        'W',
        false
      };
      player2Board[i][j] = emptyCell;
    }
  }
}

/**
 Checks if cell coordinate of type "A1", "D8" are valid as cell names and valid for this board.
 */
Coordinate Game::checkCellValidity(std::string cellStr) {
  char rowLetter = cleanUpChar(cellStr.at(0));
  int row = transformLetterToRow(rowLetter);
  if (row > config.board.height) {
    throw "Row can't be more than board height: " + std::to_string(config.board.height);
  }
  std::string colStr = cellStr.substr(1, cellStr.npos);
  int col;
  try {
    col = std::stoi(colStr);
  } catch (...) {
    throw "Coordinate column is not valid input.";
  }
  if (col > config.board.width) {
    throw "Column can't be more than board width: " + std::to_string(config.board.width);
  }
  Coordinate myCoord = {
    row,
    col
  };
  return myCoord;
}

char Game::cleanUpChar(char letter) {
  if (letter >= 'a' && letter <= 'z') {
    letter += ('A' - 'a'); 
  }

  if ((letter < 'A') || (letter > 'Z')) {
    throw "Invalid character. Rows are expressed by a letter.";
  }

  return letter;
}

int Game::transformLetterToRow(char letter) {
  return letter - 64;
}

void Game::shipPlacementPhase() {
  bool exit = false;
  for (Ship ship : config.ships) {
    std::cout << "\033[1;34m\nTime to place your " << ship.shipName << " 🚢 !\033[0m\n";
    std::cout << "\033[34mThis ship has a size of " << ship.shipSize << "\033[0m\n";
    std::cout << "\033[34mPlease, write the starting cell for your ship, i.e. A1.\nWrite 0 to skip this step and autoplace the ships.\033[0m\n";
    std::string input = readLine();
    if (input == "0") {
      std::cout << "\033[34mShips will be randomly placed.\033[0m\n";
      break;
    }
    bool validCell = false;
    Coordinate coord;
    do {
      try {
        coord = checkCellValidity(input);
        validCell = true;
      } catch (std::string error) {
        std::cout << "Error: invalid coordinate. " << error << "\nPlease try again!\n";
        input = readLine();
      }
    } while (!validCell);

    
  }
}

void Game::displayWinner() {
  std::cout << "And the winner is";
}

void Game::startGame() {
  int selection;
  do {
    displayMenu();
    try {
      selection = std::stoi(readLine());
      switch (selection) {
        case 1: {
          shipPlacementPhase();
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
    } catch (...) {
      std::cout << "That was not a valid menu option, please try again!\n";
    }
    
  } while (selection != 0);
};

void endGame();
