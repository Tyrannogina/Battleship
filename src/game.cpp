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
Game::Game() : currentPlayer(0), gameOver(false) {
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
  IOHelper::printText("Welcome to ",
                      IOHelper::BOLD,
                      IOHelper::FG_BLUE,
                      IOHelper::BG_DEFAULT,
                      false);
  IOHelper::printText("Battle",
                      IOHelper::BOLD,
                      IOHelper::FG_RED,
                      IOHelper::BG_DEFAULT,
                      false);
  IOHelper::printMenuText("ship!");
  IOHelper::printMenuText("What would you like to do?");
  IOHelper::printMenuText("1 - One player vs. computer");
  IOHelper::printMenuText("0 - Quit");
}

/**
 * Checks if cell coordinate of type "A1", "D8" are valid as cell names and
 * valid for this board.
 */
Coordinate Game::checkCellValidity(std::string cellStr) const {
  char rowLetter = cleanUpChar(cellStr.at(0));
  int row = transformLetterToRow(rowLetter);
  if (row > config.board.height) {
    throw std::length_error("Row can't be more than board height: "
                                + std::to_string(config.board.height));
  }
  std::string colStr = cellStr.substr(1, std::string::npos);
  int col;
  try {
    col = std::stoi(colStr);
  } catch (...) {
    throw std::invalid_argument("Coordinate column is not valid input.");
  }
  if (col > config.board.width) {
    throw std::length_error("Column can't be more than board width: "
                                + std::to_string(config.board.width));
  }
  Coordinate myCoord = {
      row - 1,
      col - 1
  };
  return myCoord;
}

/**
 * Checks if a character is a letter and returns it uppercase.
 * @param letter
 * @return
 */
char Game::cleanUpChar(char letter) {
  if (letter >= 'a' && letter <= 'z') {
    letter += ('A' - 'a');
  }

  if ((letter < 'A') || (letter > 'Z')) {
    throw std::invalid_argument(
        "Invalid character. Rows are expressed by a letter.");
  }

  return letter;
}

/**
 * Returns the row number for a given uppercase letter.
 * @param letter
 * @return
 */
int Game::transformLetterToRow(char letter) {
  return letter - 64;
}

/**
 * From a given cell in the board, checks in 4 directions and returns a vector
 * with those that are available to place a ship of that size.
 * @param coord
 * @param ship
 * @return
 */
std::vector<Direction> Game::getValidDirections(Coordinate coord,
                                                const int& shipSize) {
  bool isValid = true;
  std::vector<Direction> validDirections;
  char waterType = '~';

  // Checking "up"
  for (int r = coord.row - shipSize + 1; r < coord.row - 1; r++) {
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
  for (int r = coord.row + 1; r < coord.row + shipSize - 1; r++) {
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
  for (int c = coord.col + 1; c < coord.col + shipSize - 1; c++) {
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
  for (int c = coord.col - shipSize + 1; c < coord.col - 1; c++) {
    if (c < 0
        || players[currentPlayer].board.grid[coord.row][c].cellType
            != waterType) {
      isValid = false;
      break;
    }
  }
  if (isValid) {
    validDirections.push_back({"Left"});
  }

  return validDirections;
}

/**
 * Loops through each of the player's ship and asks them a starting coordinate
 * and a direction to place the ship. The player also has the option to skip
 * this step and get the ships automatically placed.
 */
void Game::manuallyPlaceShips() {
  for (const std::pair<const char, Ship>& ship : players[currentPlayer].ships) {
    IOHelper::printMenuText(
        "\nTime to place your " + ship.second.shipName + "!");
    IOHelper::printMenuText(
        "This ship has a size of " + std::to_string(ship.second.shipSize)
            + ".");
    IOHelper::printMenuText(
        "Please, write the starting cell for your ship, i.e. A1.\nWrite 0 to skip this step and autoplace the ships.");
    std::string input = IOHelper::readLine();
    if (input == "0") {
      IOHelper::printMenuText("Ships will be randomly placed.");
      autoplaceShips();
      players[currentPlayer].board.displayOwnBoard();
      break;
    }

    Coordinate coord = requestCoordinateValue(input);

    std::vector<Direction>
        validDirections = getValidDirections(coord, ship.second.shipSize);

    IOHelper::printMenuText(
        "Great, we have the starting point. Now select one of the following directions to place your ship.");
    for (int i = 0; i < validDirections.size(); i++) {
      IOHelper::printMenuText(
          std::to_string(i + 1) + " - " + validDirections[i].name);
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
        placeShip(coord, selectedDirection, ship.second);
        players[currentPlayer].board.displayOwnBoard();
        validDirection = true;
      } catch (std::string& error) {
        IOHelper::printMenuText(
            "Error: invalid direction to place the ship. " + error
                + "\nPlease try again!\n");
        input = IOHelper::readLine();
      }
    } while (!validDirection);
  }
}

/**
 * Given a user input, checks if it can be parsed to a valid position on the
 * board and keeps asking for a new input until a valid one is provided.
 *
 * @param input
 * @return
 */
Coordinate Game::requestCoordinateValue(std::string input) {
  bool validCell = false;
  Coordinate coord{};
  do {
    try {
      coord = checkCellValidity(input);
      validCell = true;
    } catch (std::string& error) {
      IOHelper::printMenuText(
          "Error: invalid coordinate. " + error + "\nPlease try again!");
      input = IOHelper::readLine();
    }
  } while (!validCell);
  return coord;
}

/**
 * Given a ship, a valid coordinate and a direction, it records the ships'
 * placement on the board.
 * @param coord
 * @param dir
 * @param ship
 */
void Game::placeShip(Coordinate coord, const Direction& dir, const Ship& ship) {
  Cell cell = {ship.shipCode, false};
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

/**
 * Selects a random coordinate from the board.
 * @return
 */
Coordinate Game::pickRandomCoordinate() const {
  return {randomInt(0, config.board.height - 1),
          randomInt(0, config.board.width - 1)};
}

/**
 * Picks one direction out of a vector of directions.
 * @param directions
 * @return
 */
Direction Game::getRandomDirection(std::vector<Direction>& directions) {
  Direction dir = directions[randomInt(0, directions.size() - 1)];
  return dir;
}

/**
 * Returns a random integer within a range (including both min and max values).
 * @param min
 * @param max
 * @return
 */
int Game::randomInt(int min, int max) {
  std::random_device rd; // only used once to initialise (seed) engine
  std::mt19937
      rng(rd()); // random-number engine used (Mersenne-Twister in this case)
  std::uniform_int_distribution<int> uni(min, max); // guaranteed unbiased
  return uni(rng);
}

/**
 * Autoplaces all existing ships on the board.
 */
//TODO: Find a way to mark ships as placed so they aren't place again if
//  the function is called after placing some ships manually.
void Game::autoplaceShips() {
  for (const std::pair<const char, Ship>& ship : config.ships) {
    Coordinate coord{};
    do {
      coord = pickRandomCoordinate();
    } while (players[currentPlayer].board.grid[coord.row][coord.col].cellType
        != '~');

    std::vector<Direction> validDirections;
    do {
      validDirections = getValidDirections(coord, ship.second.shipSize);
    } while (validDirections.empty());

    Direction dir = getRandomDirection(validDirections);
    placeShip(coord, dir, ship.second);
  }
}

/**
 * Places available ships on the board.
 */
void Game::placeShips() {
  if (players[currentPlayer].automated) {
    autoplaceShips();
  } else {
    manuallyPlaceShips();
  }
}

/**
 * Displays the winning player (either 1 or 2).
 */
void Game::displayWinner() const {
  IOHelper::printMenuText(
      "And the winner is player " + std::to_string(currentPlayer + 1));
}

/**
 * Displays initial menu directs the user to their options (at the moment,
 * one player vs computer or quit).
 */
void Game::startGame() {
  int selection;
  do {
    displayMenu();
    try {
      selection = std::stoi(IOHelper::readLine());
      switch (selection) {
        case 1: {
          shipPlacementPhase();
          turnsPhase();
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

/**
 * Runs the initial phase for placing the ships on the board for each player.
 */
void Game::shipPlacementPhase() {
  for (currentPlayer = 0; currentPlayer < players.size(); currentPlayer++) {
    placeShips();
  }
}

/**
 * Loops through the main phase of the game where turns are played by each
 * player until one of the players wins.
 */
void Game::turnsPhase() {
  currentPlayer = 0;
  int counter = 0;
  do {
    playTurn();
    switchPlayer();
    if (counter == 1) { // TODO: remove counter after testing
      gameOver = true;
    }
    counter++;
  } while (!gameOver);
}

/**
 * Switches who is the current player.
 */
void Game::switchPlayer() {
  currentPlayer = currentPlayer == 0 ? 1 : 0;
}

/**
 * Gets the enemy player.
 * @return
 */
int Game::getEnemyPlayer() const {
  return currentPlayer == 0 ? 1 : 0;
}

/**
 * Runs a turn for the player. For automated players, selects a target, for
 * manual players they have to intro the target on manually.
 * Then it records the hit and checks if the enemy was defeated and game is over
 * or not.
 */
// TODO: break down this function into atomic functions.
void Game::playTurn() {
  Coordinate coord{};

  if (players[getEnemyPlayer()].automated) {
    do {
      coord = pickRandomCoordinate();
    } while (players[getEnemyPlayer()].board.grid[coord.row][coord.col].hit);
  } else {
    IOHelper::printEndLine();
    IOHelper::printMenuText("ENEMY BOARD:");
    players[getEnemyPlayer()].board.displayEnemyBoard();
    IOHelper::printMenuText(
        "Select a coordinate to hit on your enemy's board, i.e. A1.");
    std::string input = IOHelper::readLine();
    coord = requestCoordinateValue(input);
  }

  players[getEnemyPlayer()].recordHit(coord);

  IOHelper::printMenuText("\nENEMY BOARD:");
  players[getEnemyPlayer()].board.displayEnemyBoard();

  if (players[getEnemyPlayer()].checkLost()) {
    displayWinner();
    gameOver = true;
  }
}