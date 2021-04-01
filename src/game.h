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
  Game(bool player1automation, bool player2Automation, Config config);

 private:
  Config config;
  std::vector<Player> players;
  int currentPlayer;
  bool gameOver;

  // Set-up methods
  void createPlayers(bool automatedPlayer1, bool automatedPlayer2);

  // Gameplay methods
  void shipPlacementPhase();
  void turnsPhase();
  void switchPlayer();
  void playTurn();
  int getEnemyPlayer() const;
  void displayWinner() const;
  void resetPlayers();

  // Ship placement methods
  void placeShips();
  void manuallyPlaceShips();
  void autoplaceShips();
  void autoplaceShip(Ship& ship);
  void placeShip(Coordinate coord, const Direction& dir, Ship& ship);

  // Ship placement helper methods
  void shipPlacementSelectionMenu();

  // Helper methods for direction selection
  std::vector<Direction> getValidDirections(Coordinate coord,
                                            const int& shipSize);
  static Direction getRandomDirection(std::vector<Direction>& directions);

  // Helper methods for coordinate input
  static int randomInt(int min, int max);
  Coordinate requestCoordinateValue(std::string input);
  Coordinate pickRandomCoordinate() const;
  static int transformLetterToColumn(char letter);
  static char cleanUpChar(char letter);
  Coordinate manuallySelectEmptyCoordinate(const std::string& initialMessage,
                                           const std::string& invalidCoordMessage);
  Coordinate checkCellValidity(std::string cellStr) const;


};

#endif