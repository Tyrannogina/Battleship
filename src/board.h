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
  UNKNOWN_HIT = 5
};

/// Each one of the components of the board
struct Cell {
  char cellType;
  bool hit;
};

/// A coordinate on the board.
struct Coordinate {
  int row;
  int col;
};

// Given a cell, this stores a direction (Up, Down, Left, Right)
struct Direction {
  std::string name;
};

class Board {
 private:
  int height{};
  int width{};

 public:
  Board(int height, int width);
  Board();

  std::vector<std::vector<Cell>> grid;
  static CellRepresentation assignCellRepresentationForOwnBoard(Cell& cell);
  void displayOwnBoard();
  void displayEnemyBoard();
  static void printCell(CellRepresentation rep, const Cell& cell);
  static CellRepresentation assignCellRepresentationForEnemyBoard(Cell& cell);
  static void printRowNumber(int row);
  void printColLetters() const;
};

#endif