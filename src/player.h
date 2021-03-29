#ifndef SRC_PLAYER_H
#define SRC_PLAYER_H

#include "board.h"
#include "models.h"

class Player {
 public:
  Player(bool automated);
  Player();
  Board board;
  std::map<char, Ship> ships;
  bool automated{};
  int getScore() const;
  void scorePoints(int points);
  void initialisePlayer(Config& config);
  void recordHit(Coordinate& coord);
  bool checkLost() const;

 private:
  int score;
  int remainingShips;

};

#endif //SRC_PLAYER_H
