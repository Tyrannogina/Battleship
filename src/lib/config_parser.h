#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <iostream>

#include "models.h"

class ConfigParser
{
public:
  ConfigParser();
  Config parseConfig();

private:
  const int MIN_BOARD_SIZE;
  const int MAX_BOARD_SIZE;
  const std::string filename;
  bool fileExists();
  int parseSize(std::string sizeStr);
  int parseBoardSize(std::string sizeStr);
  int parseShipSize(std::string sizeStr);
  std::vector<std::string> fileToVector();
  Board parseBoard(std::string str);
  Ship parseShip(std::string str);
};
 
#endif