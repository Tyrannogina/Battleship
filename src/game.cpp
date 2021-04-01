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
Game::Game(bool player1automation, bool player2Automation, Config config)
    : currentPlayer(0),
      gameOver(false),
      config(config) {
  createPlayers(player1automation, player2Automation);
  shipPlacementPhase();
  turnsPhase();
}

/**
 * Creates the 2 players for the Battleship game.
 * @param player1automation
 * @param player2Automation
 */
void Game::createPlayers(bool player1automation, bool player2Automation) {
  Player p1(player1automation, config);
  Player p2(player2Automation, config);
  this->players = {p1, p2};
}

/**
 * Checks if cell coordinate of type "A1", "D8" are valid as cell names and
 * valid for this board.
 */
Coordinate Game::checkCellValidity(std::string cellStr) const {
  char colLetter = cleanUpChar(cellStr.at(0));
  int col = transformLetterToColumn(colLetter);
  if (col > config.board.width) {
    throw std::length_error("Column can't be more than board width: "
                                + std::to_string(config.board.width));
  }
  int row;
  std::string rowStr = cellStr.substr(1, std::string::npos);
  try {
    row = std::stoi(rowStr);
  } catch (...) {
    throw std::invalid_argument("Invalid input for row.");
  }
  if (row > config.board.height) {
    throw std::length_error("Row can't be more than board height: "
                                + std::to_string(config.board.height));
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
        "Invalid character. Columns are expressed by a letter.");
  }

  return letter;
}

/**
 * Returns the column number for a given uppercase letter.
 * @param letter
 * @return
 */
int Game::transformLetterToColumn(char letter) {
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
  for (int r = coord.row + 1; r < coord.row + shipSize; r++) {
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
  for (int c = coord.col + 1; c < coord.col + shipSize; c++) {
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
 * Tries to pick an empty coordinate in the board from user input.
 * @param initialMessage message printed the first time asking for the
 * coordinate.
 * @param invalidCoordMessage message printed when asking if the previous
 * attempt was not valid.
 * @return Coordinate
 */
Coordinate Game::manuallySelectEmptyCoordinate(const std::string& initialMessage,
                                               const std::string& invalidCoordMessage) {
  IOHelper::printMenuText(initialMessage);
  std::string input = IOHelper::readLine();
  Coordinate coord{};
  do {
    if (input == "0") {
      // This is a workaround to allow users to automatically place ships.
      throw std::exception();
    }
    coord = requestCoordinateValue(input);
    if (players[currentPlayer].board.grid[coord.row][coord.col].cellType
        != '~') {
      IOHelper::printMenuText(invalidCoordMessage);
      input = IOHelper::readLine();
    }
  } while (players[currentPlayer].board.grid[coord.row][coord.col].cellType
      != '~');
  return coord;
}

/**
 * Loops through each of the player's ship and asks them a starting coordinate
 * and a direction to place the ship. The player also has the option to place
 * each ship automatically.
 */
void Game::manuallyPlaceShips() {
  for (std::pair<const char, Ship>& ship : players[currentPlayer].ships) {
    players[currentPlayer].displayShipPlacementStatus();
    IOHelper::printMenuText(
        "\nTime to place your " + ship.second.shipName + "!");
    IOHelper::printMenuText(
        "This ship has a size of " + std::to_string(ship.second.shipSize)
            + ".");
    Coordinate coord;
    try {
      coord = manuallySelectEmptyCoordinate(
          "Please, write the starting cell for your ship, i.e. A1.\nWrite 0 to skip this step and autoplace this ship.",
          "This cell is already occupied, please select a different one: i.e. B2");
    } catch (...) {
      IOHelper::printMenuText(
          "The " + ship.second.shipName + " will be randomly placed.");
      autoplaceShip(ship.second);
      players[currentPlayer].board.displayOwnBoard();
      continue;
    }

    Direction selectedDirection{"Up"};
    if (ship.second.shipSize > 1) {
      std::vector<Direction>
          validDirections = getValidDirections(coord, ship.second.shipSize);

      IOHelper::printMenuText(
          "Great, we have the starting point. Now select one of the following directions to place your ship.");
      for (int i = 0; i < validDirections.size(); i++) {
        IOHelper::printMenuText(
            std::to_string(i + 1) + " - " + validDirections[i].name);
      }
      IOHelper::printMenuText("0 - Place ship automatically");
      std::string input = IOHelper::readLine();

      if (input == "0") {
        IOHelper::printMenuText(
            "The " + ship.second.shipName + " will be randomly placed.");
        autoplaceShip(ship.second);
        players[currentPlayer].board.displayOwnBoard();
        continue;
      }

      bool validDirection = false;
      do {
        try {
          selectedDirection = validDirections[std::stoi(input) - 1];

          validDirection = true;
        } catch (...) {
          IOHelper::printMenuText(
              "Error: invalid direction to place the ship. Please try again!\n");
          input = IOHelper::readLine();
        }
      } while (!validDirection);
    }
    placeShip(coord, selectedDirection, ship.second);
    players[currentPlayer].board.displayOwnBoard();
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
    } catch (...) {
      IOHelper::printMenuText(
          "Error: invalid coordinate. Please try again!");
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
void Game::placeShip(Coordinate coord, const Direction& dir, Ship& ship) {
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
  ship.isPlaced = true;
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
 * Automatically places unplaced ships for a particular player on their board.
 */
void Game::autoplaceShips() {
  IOHelper::printMenuText("Ships are being placed automatically for player "
                              + std::to_string(currentPlayer + 1));
  for (std::pair<const char, Ship>& ship : players[currentPlayer].ships) {
    autoplaceShip(ship.second);
  }
}

/**
 * Automatically places a single ship into the board.
 * @param ship
 */
void Game::autoplaceShip(Ship& ship) {
  if (!ship.isPlaced) {
    Coordinate coord{};
    std::vector<Direction> validDirections;
    do {
      coord = pickRandomCoordinate();
    } while (players[currentPlayer].board.grid[coord.row][coord.col].cellType
        != '~');
    do {
      validDirections = getValidDirections(coord, ship.shipSize);
    } while (validDirections.empty());

    Direction dir = getRandomDirection(validDirections);
    placeShip(coord, dir, ship);
  }
}

/**
 * Places available ships on the board depending on type of player.
 */
void Game::placeShips() {
  if (players[currentPlayer].automated) {
    autoplaceShips();
  } else {
    bool happy = false;
    do {
      players[currentPlayer].board.displayOwnBoard();
      shipPlacementSelectionMenu();
      IOHelper::printMenuText(
          "All your ships have been placed! What would you like to do?");
      IOHelper::printMenuText("1 - Continue. Let's play!");
      IOHelper::printMenuText("2 - Reset board.");
      IOHelper::printMenuText("0 - Quit");
      std::string input = IOHelper::readLine();
      if (input == "1") {
        happy = true;
      } else if (input == "2") {
        IOHelper::printMenuText("OK, let's try placing those ships again");
        happy = false;
        resetPlayers();
      } else {
        IOHelper::printMenuText("OK, quitting the game. See you soon!");
        happy = true;
        gameOver = true;
      }
    } while (!happy);
  }
}

/**
 * Displays a menu for manual players to select if they want to place ships
 * manually or automatically on their boards.
 */
void Game::shipPlacementSelectionMenu() {
  IOHelper::printMenuText(
      "It's your turn to place the ships on the board. How would you like to do this?");
  IOHelper::printMenuText("1 - Place ships manually.");
  IOHelper::printMenuText("2 - Place ships automatically.");
  std::string input = IOHelper::readLine();
  if (input == "1") {
    manuallyPlaceShips();
  } else if (input == "2") {
    autoplaceShips();
    players[currentPlayer].board.displayOwnBoard();
  }
}

/**
 * Displays the winner (either player 1 or 2).
 */
void Game::displayWinner() const {
  IOHelper::printBoardLabelText("                                  ");
  IOHelper::printBoardLabelText("                                  ");
  IOHelper::printEndLine();
  IOHelper::printMenuText(
      "The winner is player " + std::to_string(currentPlayer + 1));
  IOHelper::printBoardLabelText("                                  ");
  IOHelper::printBoardLabelText("                                  ");
  IOHelper::printEndLine();
}

/**
 * Resets the players.
 */
void Game::resetPlayers() {
  bool p1Auto = players[0].automated;
  bool p2Auto = players[1].automated;
  Player player1(p1Auto, config);
  Player player2(p2Auto, config);
  players = {player1, player2};
}

/**
 * Runs the initial phase for placing the ships on the board for each player.
 */
void Game::shipPlacementPhase() {
  for (currentPlayer = 0; currentPlayer < players.size(); currentPlayer++) {
    if (!gameOver) {
      placeShips();
    }
  }
}

/**
 * Loops through the main phase of the game where turns are played by each
 * player until one of the players wins.
 */
void Game::turnsPhase() {
  currentPlayer = 0;
  while (!gameOver) {
    playTurn();
    switchPlayer();
  };
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
void Game::playTurn() {
  IOHelper::printEndLine();
  IOHelper::printMenuText(
      "PLAYER " + std::to_string(currentPlayer + 1) + "'S TURN:");
  Coordinate coord{};

  if (players[currentPlayer].automated) {
    do {
      coord = pickRandomCoordinate();
    } while (players[getEnemyPlayer()].board.grid[coord.row][coord.col].hit);
  } else {
    players[currentPlayer].board.displayOwnBoard();
    IOHelper::printEndLine();
    IOHelper::printMenuText(
        "ENEMY (PLAYER " + std::to_string(getEnemyPlayer() + 1) + ") BOARD:");
    players[getEnemyPlayer()].board.displayEnemyBoard();
    IOHelper::printMenuText(
        "Select a coordinate to fire upon your enemy's board, i.e. A1.");
    IOHelper::printMenuText(
        "Alternatively, select 1 for autofire or 0 to quit).");
    std::string input = IOHelper::readLine();
    if (input == "0") {
      IOHelper::printMenuText("OK, quitting the game. See you soon!");
      gameOver = true;
      return;
    } else if (input == "1") {
      coord = pickRandomCoordinate();
    } else {
      coord = requestCoordinateValue(input);
    }
  }
  IOHelper::printMenuText(
      "Player " + std::to_string(currentPlayer + 1) + " fired at player "
          + std::to_string(getEnemyPlayer() + 1) + "!!");
  players[getEnemyPlayer()].recordHit(coord);

  if (players[getEnemyPlayer()].checkLost()) {
    displayWinner();
    gameOver = true;
  } else {
    IOHelper::printMenuText(
        "PLAYER " + std::to_string(currentPlayer + 1) + "'S SCORE: "
            + std::to_string(players[currentPlayer].getScore()));
    IOHelper::printMenuText(
        "PLAYER " + std::to_string(getEnemyPlayer() + 1) + "'S SCORE: "
            + std::to_string(players[getEnemyPlayer()].getScore()));
    IOHelper::printMenuText(
        "END OF PLAYER " + std::to_string(currentPlayer + 1) + "'S TURN");
  }
}

