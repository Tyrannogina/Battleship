#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
/// Each one of the components of the board
struct Cell {
  char cellType;
  bool firedUpon;
  std::string representation;
};

class Board {
 private:
  int height;
  int width;

 public:
  Board(int height, int width);
  Board();

  std::vector<std::vector<Cell>> grid;
  int getHeight() const;
  int getWidth() const;
};

#endif