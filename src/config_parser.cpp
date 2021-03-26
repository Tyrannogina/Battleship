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
      filename("adaship_config.ini") {};

/**
  * Checks if the config file exists.
  */
bool ConfigParser::configFileExists() {
  struct stat buf;
  if (stat(filename.c_str(), &buf) == 0) {
    return true;
  }
  return false;
}

/**
  * Splits a string by a single delimiter and place the new strings in a vector.
  * @param delimiter - Delimiter string
  * @param str - String to be split
  * @return vector with the two strings resulting from the split.
  */
std::vector<std::string> splitByDelimiter(std::string delimiter,
                                          std::string str) {
  int delimiterPos = str.find(delimiter);
  if (delimiterPos == str.npos) {
    throw "Error: format of configuration line not recognised";
  }
  std::vector<std::string> vect;
  vect.push_back(str.substr(0, delimiterPos));
  vect.push_back(str.substr(delimiterPos + delimiter.size(), str.npos));
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
int ConfigParser::sizeValidator(std::string sizeStr) {
  return std::stoi(sizeStr);
}

/**
  * Checks if a string actually contains an integer and if said integer 
  * @param sizeStr - string to validate
  * @return int
  */
int ConfigParser::parseBoardSize(std::string sizeStr) {
  try {
    int size = sizeValidator(sizeStr);
    if (size > MAX_BOARD_SIZE) {
      size = MAX_BOARD_SIZE;
    } else if (size < MIN_BOARD_SIZE) {
      size = MIN_BOARD_SIZE;
    }
    return size;
  } catch (...) {
    throw "Invalid format for the board size";
  }
}

int ConfigParser::parseShipSize(std::string sizeStr) {
  int size = sizeValidator(sizeStr);
  if (size > MAX_BOARD_SIZE) {
    throw "Ship can't be bigger than the board";
  } else if (size < 1) {
    throw "Ship size can't be less than 1";
  }
  return size;
}

BoardConfig ConfigParser::parseBoard(std::string str) {
  std::vector<std::string> sizes = splitByDelimiter("x", str);
  return {
      parseBoardSize(sizes[0]),
      parseBoardSize(sizes[1])
  };
}

Ship ConfigParser::parseShip(std::string shipStr) {
  std::vector<std::string> shipVector = splitByDelimiter(",", shipStr);
  Ship ship = {
      shipVector[0].at(0),
      shipVector[0],
      parseShipSize(shipVector[1])
  };
  return ship;
}

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

Config ConfigParser::parseConfig() {
  if (!configFileExists()) {
    std::cout << "File not found";
  }
  std::vector<std::string> configLines = fileToVector();
  Config config;

  for (std::string line : configLines) {
    removeSpaces(line);
    std::vector<std::string> parsedLine = splitByDelimiter(":", line);
    if (parsedLine[0] == "Board") {
      config.board = parseBoard(parsedLine[1]);
    }
    if (parsedLine[0] == "Boat") {
      // TODO: insert control of repeated ships. For now we trust they don't repeat the ID
      Ship currentShip = parseShip(parsedLine[1]);
      config.ships.push_back(currentShip);
    }
  }
  return config;
}