#include <iostream>
#include "board.h"
#include "IOHelper.h"

Board::Board() {}

Board::Board(int height, int width) : height(height), width(width) {
  Cell emptyCell = {'~', false};
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

void Board::displayOwnBoard() {
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      CellRepresentation rep = assignCellRepresentationForOwnBoard(grid[row][col]);
      printCell(rep, grid[row][col]);
    }
    IOHelper::printEndLine();
  }
}

void Board::displayEnemyBoard() {
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      if (grid[row][col].hit) {
        //std::cout << grid[row][col].representation;
      }
    }
    IOHelper::printEndLine();
  }
}

void Board::printCell(CellRepresentation rep, const Cell& cell) {
  switch (rep) {
    case CellRepresentation::WATER:
      IOHelper::printText("[~]",
                          IOHelper::REGULAR,
                          IOHelper::FG_BLUE,
                          IOHelper::BG_DEFAULT,
                          false);
      break;
    case CellRepresentation::WATER_HIT:
      IOHelper::printText("[~]",
                          IOHelper::REGULAR,
                          IOHelper::FG_DEFAULT,
                          IOHelper::BG_BLUE,
                          false);
      break;
    case CellRepresentation::UNKNOWN:
      IOHelper::printText("[.]",
                          IOHelper::REGULAR,
                          IOHelper::FG_DEFAULT,
                          IOHelper::BG_DEFAULT,
                          false);
      break;
    case CellRepresentation::SHIP: {
      std::string s(1, cell.cellType);
      IOHelper::printText("[" + s + "]",
                          IOHelper::BOLD,
                          IOHelper::FG_DEFAULT,
                          IOHelper::BG_DEFAULT,
                          false);
    }
      break;
    case CellRepresentation::SHIP_HIT: {
      std::string s(1, cell.cellType);
      IOHelper::printText("[" + s + "]",
                          IOHelper::REGULAR,
                          IOHelper::FG_RED,
                          IOHelper::BG_DEFAULT,
                          false);
    }
      break;
  }
}

CellRepresentation Board::assignCellRepresentationForOwnBoard(Cell& cell) {
  if (cell.cellType == '~') {
    if (cell.hit) {
      return WATER_HIT;
    } else {
      return WATER;
    }
  } else {
    if (cell.hit) {
      return SHIP_HIT;
    } else {
      return SHIP;
    }
  }
}
