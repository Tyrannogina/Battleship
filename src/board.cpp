#include <iostream>
#include "board.h"
#include "IOHelper.h"

Board::Board() = default;

Board::Board(int height, int width) : height(height), width(width) {
  Cell emptyCell = {'~', false};
  this->grid = std::vector<std::vector<Cell>>(
      height,
      std::vector<Cell>(width, emptyCell));
}

/**
 * Prints a letter label for a particular row.
 * @param {int} row
 */
void Board::printRowLetter(int row) {
  std::string rowLabels = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  std::string s(1, rowLabels.at(row));
  IOHelper::printBoardLabelText(" " + s + " ");
}

/**
 * Prints a row with all the column labels.
 */
void Board::printColNumbers() const {
  IOHelper::printBoardLabelText("   ");
  for (int col = 1; col <= width; col++) {
    IOHelper::printBoardLabelText(" " + std::to_string(col) + " ");
  }
  IOHelper::printEndLine();
}

/**
 * Displays a player own board: all ships are visible by default.
 */
void Board::displayOwnBoard() {
  printColNumbers();
  for (int row = 0; row < height; row++) {
    printRowLetter(row);
    for (int col = 0; col < width; col++) {
      CellRepresentation
          rep = assignCellRepresentationForOwnBoard(grid[row][col]);
      printCell(rep, grid[row][col]);
    }
    IOHelper::printEndLine();
  }
}

/**
 * Displays the enemy board. Only cells that have been hit display the content
 * (either ship or water).
 */
void Board::displayEnemyBoard() {
  printColNumbers();
  for (int row = 0; row < height; row++) {
    printRowLetter(row);
    for (int col = 0; col < width; col++) {
      CellRepresentation
          rep = assignCellRepresentationForEnemyBoard(grid[row][col]);
      printCell(rep, grid[row][col]);
    }
    IOHelper::printEndLine();
  }
}

/**
 * Prints a particular cell of the board according to the representation assigned.
 * @param {enum} rep
 * @param cell
 */
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
                          IOHelper::FG_WHITE,
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
    case CellRepresentation::UNKNOWN_HIT: {
      std::string s(1, cell.cellType);
      IOHelper::printText("[ # ]",
                          IOHelper::REGULAR,
                          IOHelper::FG_RED,
                          IOHelper::BG_DEFAULT,
                          false);
    }
      break;
  }
}

/**
 * Returns a particular representation type to a player's board cell.
 * @param {Cell} cell
 * @return CellRepresentation
 */
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

/**
 * Returns a particular representation type to an enemy board cell.
 * @param cell
 * @return CellRepresentation
 */
CellRepresentation Board::assignCellRepresentationForEnemyBoard(Cell& cell) {
  if (cell.hit) {
    if (cell.cellType == '~') {
      return WATER_HIT;
    } else {
      return UNKNOWN_HIT;
    }
  } else {
    return UNKNOWN;
  }
}