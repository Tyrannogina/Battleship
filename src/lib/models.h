#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>

struct Ship {
  char shipId;
  std::string shipName;
  int shipSize;
};

struct BoardConfig {
  int height;
  int width;
};

struct Config {
  BoardConfig board;
  std::vector<Ship> ships;
};

#endif
