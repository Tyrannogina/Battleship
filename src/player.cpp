#include "player.h"
#include "IOHelper.h"

Player::Player() = default;

Player::Player(bool automated) : automated(automated), score(0) {
}

int Player::getScore() const {
  return score;
}

/**
 * Adds points to the score.
 * @param points
 */
void Player::scorePoints(int points) {
  score += points;
}

/**
 * Performs necessary operations to initialise a player: create board, get ships
 * from the config.
 * @param config
 */
void Player::initialisePlayer(Config& config) {
  this->board = Board(config.board.height, config.board.width);
  this->ships = config.ships;
  this->remainingShips = config.totalShips;
}

/**
 * Records a hit on a cell.
 * If the cell contains a boat, it records the hit on the ship and adds 5 points
 * to the score.
 * If the boat is sunk, it scores 10 extra points and removes the ship from the
 * remaining ships count.
 * @param coord
 */
void Player::recordHit(Coordinate &coord) {
  Cell* cell = &board.grid[coord.row][coord.col];
  cell->hit = true;
  if (cell->cellType != '~') {
    ships[cell->cellType].shipHits++;
    scorePoints(5);
    if (ships[cell->cellType].shipHits == ships[cell->cellType].shipSize) {
      remainingShips--;
      ships[cell->cellType].shipName == "Giant Platypus"? scorePoints(50) : scorePoints(10);
      IOHelper::printContrastMenuText("The " + ships[cell->cellType].shipName + " was sunk!");
    } else {
      IOHelper::printContrastMenuText("A ship was hit!");
    }
  } else {
    IOHelper::printContrastMenuText("It's a miss :(");
  }
}

/**
 * Checks if a player has lost by checking the remaining ships.
 * @return bool
 */
bool Player::checkLost() const {
  return remainingShips == 0;
}

/**
 * Displays a list of all the ships and if they are placed or not.
 */
void Player::displayShipPlacementStatus() {
  for (std::pair<char, Ship> ship : ships) {
    std::string placement = "Not placed";
    if (ship.second.isPlaced) {
      placement = "Placed";
    }
    IOHelper::printMenuText(ship.second.shipName + ": " + placement);
  }
};