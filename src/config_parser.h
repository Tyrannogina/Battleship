#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <iostream>

#include "models.h"

/**
  * Parses a config file for the game containing lines describing the board size and the ships.
  * "Board: 10x10"
  * "Boat: Carrier, 5"
  * and as many boats as are on the board.
  */
class ConfigParser {
 public:
  ConfigParser();
  Config parseConfig();

 private:
  const int MIN_BOARD_SIZE;
  const int MAX_BOARD_SIZE;
  const std::string filename;
  bool configFileExists();
  int sizeValidator(std::string sizeStr);
  int parseBoardSize(std::string sizeStr);
  int parseShipSize(std::string sizeStr);
  std::vector<std::string> fileToVector();
  BoardConfig parseBoard(std::string str);
  Ship parseShip(std::string str);
};

#endif