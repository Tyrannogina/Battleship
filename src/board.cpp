#include <iostream>
#include "board.h"

Board::Board() {}

Board::Board(int height, int width) : height(height), width(width) {
  Cell emptyCell = {'~', false, "[~]"};
  this->grid = std::vector<std::vector<Cell>>(
      height,
      std::vector<Cell>(width, emptyCell));
}

int Board::getHeight() const {
  return height;
}

int Board::getWidth() const {
  return width;
}

void Board::displayBoard() {
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      std::cout << grid[row][col].representation;
    }
    std::cout << std::endl;
  }
}
