#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>
#include <map>

struct Ship {
  char shipCode;
  std::string shipName;
  int shipSize;
  int shipHits;
  bool isPlaced;
};

struct BoardConfig {
  int height;
  int width;
};

struct Config {
  BoardConfig board;
  std::map<char, Ship> ships;
  int totalShips;
};

#endif //MODELS_H