#ifndef GAME_H
#define GAME_H

#include "lib/models.h"

/// Each one of the components of the board
struct Cell {
  char cellType;
  bool firedUpon;
};

/// A coordinate on the board.
struct Coordinate {
  int row;
  int col;
};

// Given a cell, this stores a direction (up, down, left, right) and if it's free to place a boat
struct Direction {
  std::string name;
  bool state;
};

/**
  Contains the Game class, which is in charge of running the game, from selection of game type, initialisation, turns and game until end.
 */
class Game
{
public:
  Game();
  ~Game();
  void startGame();
  void endGame();

private:
  /// Config information containing board size and ships information.
  Config config;
  /// Board of the first player, to be used for the duration of the game
  Cell **player1Board;
  /// Board of the second player, to be used for the duration of the game 
  Cell **player2Board;

  void displayMenu();
  void displayShipPlacementMenu(Ship currentShip);
  int readMenuSelection();
  void createPlayer1Board();
  void createPlayer2Board();
  void shipPlacementPhase();
  char cleanUpChar(char letter);
  Coordinate checkCellValidity(std::string cellStr);
  int transformLetterToRow(char letter);
  std::vector<Direction> getValidDirections(Coordinate coord, Ship ship);
  void placeShip(Coordinate coord, Direction dir, Ship ship);
  void autoplaceShips();
  Coordinate pickRandomCoordinate();
  Direction getRandomDirection(std::vector<Direction>& directions);
  int randomInt(int min, int max);
  void displayWinner();
};    

#endif