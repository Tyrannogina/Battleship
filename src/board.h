#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>

enum CellRepresentation {
  WATER = 0,
  WATER_HIT = 1,
  UNKNOWN = 2,
  SHIP = 3,
  SHIP_HIT = 4,
};

/// Each one of the components of the board
struct Cell {
  char cellType;
  bool hit;
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
  static CellRepresentation assignCellRepresentationForOwnBoard(Cell& cell);
  void displayOwnBoard();
  void displayEnemyBoard();
  void printCell(CellRepresentation rep, const Cell& cell);
};

#endif