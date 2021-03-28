#include "player.h"

Player::Player(bool automated) : automated(automated) {
}

int Player::getScore() {
  return score;
}
void Player::scorePoints(int points) {
  score += points;
}
void Player::initialisePlayer(Config& config) {
  this->board = Board(config.board.height, config.board.width);
  this->ships = config.ships;
  score = 0;
}

Player::Player() {}
