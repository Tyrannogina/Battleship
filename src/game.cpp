#include <iostream>
#include <string>
#include <random>
#include <vector>

#include "config_parser.h"
#include "board.h"
#include "game.h"
#include "IOHelper.h"

/**
 * Constructor calls config parser and creates players based on the config.
 */
Game::Game() : currentPlayer(0) {
  ConfigParser configParser;
  config = configParser.parseConfig();
  createPlayers(false, true);
}

void Game::createPlayers(bool automatedPlayer1, bool automatedPlayer2) {
  Player player1(automatedPlayer1);
  player1.initialisePlayer(config);
  Player player2(automatedPlayer2);
  player2.initialisePlayer(config);
  this->players = {player1, player2};
}

/**
  * Displays initial menu with game options and option for quitting.
  */
void Game::displayMenu() {
  IOHelper::printMenuText("Welcome to Battleship 💥 🚢 !");
  IOHelper::printMenuText("What would you like to do?");
  IOHelper::printMenuText("1 - One player vs. computer");
  IOHelper::printMenuText("0 - Quit");
}

/**
 Checks if cell coordinate of type "A1", "D8" are valid as cell names and valid for this board.
 */
Coordinate Game::checkCellValidity(std::string cellStr) {
  char rowLetter = cleanUpChar(cellStr.at(0));
  int row = transformLetterToRow(rowLetter);
  if (row > config.board.height) {
    throw "Row can't be more than board height: "
        + std::to_string(config.board.height);
  }
  std::string colStr = cellStr.substr(1, std::string::npos);
  int col;
  try {
    col = std::stoi(colStr);
  } catch (...) {
    throw "Coordinate column is not valid input.";
  }
  if (col > config.board.width) {
    throw "Column can't be more than board width: "
        + std::to_string(config.board.width);
  }
  Coordinate myCoord = {
      row - 1,
      col - 1
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

std::vector<Direction> Game::getValidDirections(Coordinate coord,
                                                const Ship& ship) {
  bool isValid = true;
  std::vector<Direction> validDirections;
  char waterType = '~';

  // Checking "up"
  for (int r = coord.row - ship.shipSize + 1; r < coord.row - 1; r++) {
    if (r < 0
        || players[currentPlayer].board.grid[r][coord.col].cellType
            != waterType) {
      isValid = false;
      break;
    }
  }
  if (isValid) {
    validDirections.push_back({"Up"});
  } else {
    isValid = true;
  }

  // Check "down"
  for (int r = coord.row + 1; r < coord.row + ship.shipSize - 1; r++) {
    if (r > config.board.height - 1
        || players[currentPlayer].board.grid[r][coord.col].cellType
            != waterType) {
      isValid = false;
      break;
    }
  }
  if (isValid) {
    validDirections.push_back({"Down"});
  } else {
    isValid = true;
  }

  // Check "right"
  for (int c = coord.col + 1; c < coord.col + ship.shipSize - 1; c++) {
    if (c > config.board.width - 1
        || players[currentPlayer].board.grid[coord.row][c].cellType
            != waterType) {
      isValid = false;
      break;
    }
  }
  if (isValid) {
    validDirections.push_back({"Right"});
  } else {
    isValid = true;
  }

  // Check "left"
  for (int c = coord.col - ship.shipSize + 1; c < coord.col - 1; c++) {
    if (c < 0
        || players[currentPlayer].board.grid[coord.row][c].cellType
            != waterType) {
      isValid = false;
      break;
    }
  }
  if (isValid) {
    validDirections.push_back({"Left"});
  } else {
    isValid = true;
  }

  return validDirections;
}

void Game::manuallyPlaceShips() {
  for (const Ship& ship : config.ships) {
    IOHelper::printMenuText("\nTime to place your " + ship.shipName + " 🚢 !");
    IOHelper::printMenuText("This ship has a size of " + std::to_string(ship.shipSize));
    IOHelper::printMenuText(
        "Please, write the starting cell for your ship, i.e. A1.\nWrite 0 to skip this step and autoplace the ships.");
    std::string input = IOHelper::readLine();
    if (input == "0") {
      IOHelper::printMenuText("Ships will be randomly placed.");
      autoplaceShips();
      players[currentPlayer].board.displayOwnBoard();
      break;
    }
    bool validCell = false;
    Coordinate coord{};
    do {
      try {
        coord = checkCellValidity(input);
        validCell = true;
      } catch (std::string& error) {
        IOHelper::printMenuText("Error: invalid coordinate. " + error + "\nPlease try again!");
        input = IOHelper::readLine();
      }
    } while (!validCell);

    std::vector<Direction> validDirections = getValidDirections(coord, ship);

    IOHelper::printMenuText("Great, we have the starting point. Now select one of the following directions to place your ship.");
    for (int i = 0; i < validDirections.size(); i++) {
      IOHelper::printMenuText(std::to_string(i + 1) + " - " + validDirections[i].name);
    }
    IOHelper::printMenuText("0 - Quit");
    input = IOHelper::readLine();

    if (input == "0") {
      IOHelper::printMenuText("Ships will be randomly placed.");
      autoplaceShips();
      players[currentPlayer].board.displayOwnBoard();
      break;
    }

    bool validDirection = false;
    do {
      try {
        Direction selectedDirection = validDirections[std::stoi(input) - 1];
        placeShip(coord, selectedDirection, ship);
        players[currentPlayer].board.displayOwnBoard();
        validDirection = true;
      } catch (std::string error) {
        IOHelper::printMenuText(
            "Error: invalid direction to place the ship. " + error
                                                           + "\nPlease try again!\n");
        input = IOHelper::readLine();
      }
    } while (!validDirection);

  }
}

void Game::placeShip(Coordinate coord, Direction dir, Ship ship) {
  Cell cell = {ship.shipId, false};
  for (int i = 0; i < ship.shipSize; i++) {
    if (dir.name == "Up")
      players[currentPlayer].board.grid[coord.row - i][coord.col] = cell;
    if (dir.name == "Down")
      players[currentPlayer].board.grid[coord.row + i][coord.col] = cell;
    if (dir.name == "Left")
      players[currentPlayer].board.grid[coord.row][coord.col - i] = cell;
    if (dir.name == "Right")
      players[currentPlayer].board.grid[coord.row][coord.col + i] = cell;
  }
}

Coordinate Game::pickRandomCoordinate() {
  return {randomInt(0, config.board.height - 1),
          randomInt(0, config.board.width - 1)};
}

Direction Game::getRandomDirection(std::vector<Direction>& directions) {
  Direction dir = directions[randomInt(0, directions.size() - 1)];
  return dir;
}

int Game::randomInt(int min, int max) {
  std::random_device rd; // only used once to initialise (seed) engine
  std::mt19937
      rng(rd()); // random-number engine used (Mersenne-Twister in this case)
  std::uniform_int_distribution<int> uni(min, max); // guaranteed unbiased
  return uni(rng);
}
void Game::autoplaceShips() {
  for (Ship& ship : config.ships) {
    Coordinate coord;
    do {
      coord = pickRandomCoordinate();
    } while (players[currentPlayer].board.grid[coord.row][coord.col].cellType
        != '~');

    std::vector<Direction> validDirections;
    do {
      validDirections = getValidDirections(coord, ship);
    } while (validDirections.empty());

    Direction dir = getRandomDirection(validDirections);
    placeShip(coord, dir, ship);
  }
}

void Game::placeShips() {
  if (players[currentPlayer].automated) {
    autoplaceShips();
  } else {
    manuallyPlaceShips();
  }
}

void Game::displayWinner() {
  IOHelper::printMenuText("And the winner is");
}

void Game::startGame() {
  int selection;
  do {
    displayMenu();
    try {
      selection = std::stoi(IOHelper::readLine());
      switch (selection) {
        case 1: {
          shipPlacementPhase();
          break;
        }
        case 2:break;
        case 0:IOHelper::printMenuText("Goodbye");
          break;
        default:
          IOHelper::printMenuText(
              "That was not a valid menu option, please try again!");
      }
    } catch (...) {
      IOHelper::printMenuText(
          "That was not a valid menu option, please try again!");
    }
  } while (selection != 0);
}
void Game::shipPlacementPhase() {
  for (currentPlayer = 0; currentPlayer < players.size(); currentPlayer++) {
    placeShips();
  }
}

void endGame();
