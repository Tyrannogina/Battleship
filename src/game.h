#ifndef GAME_H
#define GAME_H

#include "models.h"
#include "board.h"
#include "player.h"

/**
 * Contains the Game class, which is in charge of running the game, from selection of game type, initialisation, turns and game until end.
 */
class Game {
 public:
  Game();
  void startGame();

 private:
  Config config;
  std::vector<Player> players;
  int currentPlayer;
  bool gameOver;

  static void displayMenu();
  void createPlayers(bool automatedPlayer1, bool automatedPlayer2);
  void placeShips();
  void manuallyPlaceShips();
  void autoplaceShips();
  static char cleanUpChar(char letter);
  Coordinate checkCellValidity(std::string cellStr) const;
  static int transformLetterToColumn(char letter);
  std::vector<Direction> getValidDirections(Coordinate coord, const int& shipSize);
  void placeShip(Coordinate coord, const Direction& dir, Ship& ship);

  Coordinate pickRandomCoordinate() const;
  static Direction getRandomDirection(std::vector<Direction>& directions);
  static int randomInt(int min, int max);
  void displayWinner() const;
  void shipPlacementPhase();
  void turnsPhase();
  void switchPlayer();
  void playTurn();
  Coordinate requestCoordinateValue(std::string input);
  int getEnemyPlayer() const;
  Coordinate manuallySelectEmptyCoordinate(const std::string& initialMessage,
                                           const std::string& invalidCoordMessage);
  void resetPlayers();
  void autoplaceShip(Ship& ship);
  void shipPlacementSelection();
};

#endif