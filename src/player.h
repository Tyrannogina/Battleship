#ifndef SRC_PLAYER_H
#define SRC_PLAYER_H

#include "board.h"
#include "models.h"

class Player {
 public:
  Player(bool automated);
  Player();
  Board board;
  bool automated{};
  int getScore();
  void scorePoints(int points);
  void initialisePlayer(Config& config);

 private:
  int score{};
};

#endif //SRC_PLAYER_H
