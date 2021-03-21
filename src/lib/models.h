#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>

struct Ship {
  char boatId;
  std::string boatName;
  int boatSize;
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
