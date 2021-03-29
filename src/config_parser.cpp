#include <algorithm>
#include <fstream>
#include <sys/stat.h>
#include <string>

#include "config_parser.h"

/**
  * Constructor defines the constants for minimum and maximum board size and the filename of the configuration file.
  * File must be placed on the root of the project.
  */
ConfigParser::ConfigParser()
    : MIN_BOARD_SIZE(5),
      MAX_BOARD_SIZE(80),
      filename("../adaship_config.ini") {};

/**
 * Checks if the config file exists.
 */
bool ConfigParser::configFileExists() {
  struct stat buf;
  return stat(filename.c_str(), &buf) == 0;
}

/**
 * Splits a string by a single delimiter and place the new strings in a vector.
 * @param delimiter - Delimiter string
 * @param str - String to be split
 * @return vector with the two strings resulting from the split.
 */
std::vector<std::string> splitByDelimiter(const std::string& delimiter,
                                          const std::string& str) {
  int delimiterPos = str.find(delimiter);
  if (delimiterPos == std::string::npos) {
    throw std::invalid_argument("Error: format of configuration line not recognised");
  }
  std::vector<std::string> vect;
  vect.push_back(str.substr(0, delimiterPos));
  vect.push_back(str.substr(delimiterPos + delimiter.size(),
                            std::string::npos));
  return vect;
}

/**
 * Removes spaces from a string.
 * @param str reference to the string that will be stripped out of spaces.
 */
void removeSpaces(std::string& str) {
  str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
}

/**
 * Checks if a string actually contains an integer. Returns the integer or throws error
 * @param sizeStr - string to validate
 * @return int
 */
int ConfigParser::sizeValidator(const std::string& sizeStr) {
  return std::stoi(sizeStr);
}

/**
 * Checks if a string actually contains an integer and if said integer
 * @param sizeStr - string to validate
 * @return int
 */
int ConfigParser::parseBoardSize(const std::string& sizeStr) const {
  try {
    int size = sizeValidator(sizeStr);
    if (size > MAX_BOARD_SIZE) {
      size = MAX_BOARD_SIZE;
    } else if (size < MIN_BOARD_SIZE) {
      size = MIN_BOARD_SIZE;
    }
    return size;
  } catch (...) {
    throw std::invalid_argument("Invalid format for the board size");
  }
}
/**
 * Checks the ship size. Values need to be between 1 and the size of the board.
 * @param sizeStr
 * @return
 */
int ConfigParser::parseShipSize(const std::string& sizeStr,
                                int& boardSize) const {
  int size = sizeValidator(sizeStr);
  if (size > boardSize) {
    throw std::length_error("Ship can't be bigger than the board");
  } else if (size < 1) {
    throw std::length_error("Ship size can't be less than 1");
  }
  return size;
}

/**
 * Parses a line like "10x10" into a BoardConfig struct.
 * @param str
 * @return BoardConfig
 */
BoardConfig ConfigParser::parseBoard(const std::string& str) {
  std::vector<std::string> sizes = splitByDelimiter("x", str);
  return {
      parseBoardSize(sizes[0]),
      parseBoardSize(sizes[1])
  };
}

/**
 * Parses a line like "Battleship,4" or "Patrol Boat,2" into a Ship struct.
 * @param shipStr
 * @param boardSize
 * @return Ship
 */
Ship ConfigParser::parseShip(const std::string& shipStr, int& boardSize) {
  std::vector<std::string> shipVector = splitByDelimiter(",", shipStr);
  Ship ship = {
      shipVector[0].at(0),
      shipVector[0],
      parseShipSize(shipVector[1], boardSize),
      0
  };
  return ship;
}

/**
 * Parses a file into a vector containing each line of the file.
 * @return
 */
std::vector<std::string> ConfigParser::fileToVector() {
  std::string line;
  std::vector<std::string> parsedFile;
  std::ifstream fileobject;
  fileobject.open(filename);

  while (!fileobject.eof()) {
    getline(fileobject, line);
    parsedFile.push_back(line);
  }

  fileobject.close();
  return parsedFile;
}

/**
 * Parses the config file defined on the filename attribute into the Config
 * struct defined on the models.
 * Expected lines would be:
 * "Board: 10x10"
 * "Boat: Carrier, 5"
 *
 * @return
 */
 // TODO:
 //  KNOWN ISSUES:
 //  - Requires the board to be first in the config file so we
 //  can check if a ship will fit within the game board.
 //  - Requires the ships to have a unique starting letter in the name, else they
 //  will be rewritten.
Config ConfigParser::parseConfig() {
  if (!configFileExists()) {
    std::cout << "File not found";
  }
  std::vector<std::string> configLines = fileToVector();
  Config config;
  config.totalShips = 0;
  for (std::string line : configLines) {
    removeSpaces(line);
    std::vector<std::string> parsedLine = splitByDelimiter(":", line);
    if (parsedLine[0] == "Board") {
      config.board = parseBoard(parsedLine[1]);
    }
    if (parsedLine[0] == "Boat") {
      int maxBoardSize =
          config.board.width >= config.board.height ? config.board.width
                                                    : config.board.height;
      Ship currentShip = parseShip(parsedLine[1], maxBoardSize);
      config.ships[currentShip.shipCode] = currentShip;
      config.totalShips++;
    }
  }
  return config;
}