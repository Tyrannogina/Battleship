#ifndef BOARD_H
#define BOARD_H

/// Each one of the components of the board
struct Cell {
  char cellType;
  bool firedUpon;
};

class Board {
 private:
  int height;
  int width;
  void setHeight(int height);
  void setWidth(int width);

 public:
  Board(int height, int width);
  ~Board();

  Cell** board;
  int getHeight();
  int getWidth();
};

#endif