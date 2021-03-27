#ifndef GAME_H
#define GAME_H

#include "models.h"
#include "board.h"
#include "player.h"

/// A coordinate on the board.
struct Coordinate {
  int row;
  int col;
};

// Given a cell, this stores a direction (Up, Down, Left, Right)
struct Direction {
  std::string name;
};

/**
 * Contains the Game class, which is in charge of running the game, from selection of game type, initialisation, turns and game until end.
 */
class Game {
 public:
  Game();
  void startGame();
  void endGame();

 private:
  /// Config information containing board size and ships information.
  Config config;
  std::vector<Player> players;
  int currentPlayer;

  static void displayMenu();
  void createPlayers(bool automatedPlayer1, bool automatedPlayer2);
  void placeShips();
  void manuallyPlaceShips();
  void autoplaceShips();
  char cleanUpChar(char letter);
  Coordinate checkCellValidity(std::string cellStr);
  int transformLetterToRow(char letter);
  std::vector<Direction> getValidDirections(Coordinate coord, const Ship& ship);
  void placeShip(Coordinate coord, Direction dir, Ship ship);

  Coordinate pickRandomCoordinate();
  Direction getRandomDirection(std::vector<Direction>& directions);
  int randomInt(int min, int max);
  void displayWinner();
  void shipPlacementPhase();
};

#endif