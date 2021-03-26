#include "board.h"

Board::Board() {}

Board::Board(int height, int width) : height(height), width(width) {
  this->grid = std::vector<std::vector<Cell>>(
    height,
    std::vector<Cell>(width, {'W', false, "W"}));
}

int Board::getHeight() const {
  return height;
}

int Board::getWidth() const {
  return width;
}
