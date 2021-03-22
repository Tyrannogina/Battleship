#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>

struct Ship {
  char shipId;
  std::string shipName;
  int shipSize;
};

struct Board {
  int height;
  int width;
};

struct Config {
  Board board;
  std::vector<Ship> ships;
};

#endif
