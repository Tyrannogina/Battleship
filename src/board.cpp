#include "board.h"

Board::Board(int height, int width) {
  board = new Cell*[height];

  for (int i = 0; i < height; i++) {
    board[i] = new Cell[width];
    for (int j = 0; j < width; j++) {
      Cell emptyCell = {
        'W',
        false
      };
      board[i][j] = emptyCell;
    }
  }
}

/// Destructor destroys the boards
Board::~Board() {
  delete board;
}