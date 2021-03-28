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
  static int sizeValidator(const std::string& sizeStr);
  int parseBoardSize(const std::string& sizeStr) const;
  int parseShipSize(const std::string& sizeStr) const;
  std::vector<std::string> fileToVector();
  BoardConfig parseBoard(const std::string& str);
  Ship parseShip(const std::string& str);
};

#endif