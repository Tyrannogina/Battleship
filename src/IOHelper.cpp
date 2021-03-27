#include <iostream>
#include "IOHelper.h"

IOHelper::IOHelper() = default;

void IOHelper::printMenuText(const std::string& str) {
  printText(str, BOLD, FG_BLUE, BG_DEFAULT,true);
}

void IOHelper::printText(const std::string& str,
                                const std::string& boldness,
                                const std::string& foreground,
                                const std::string& background,
                                bool endl) {
  std::cout << "\033[" << boldness << ";" << background << ";" << foreground << "m";
  std::cout << str;// << "\033[" << REGULAR << "m";
  if (endl) printEndLine();
}

/**
  * Reads a line inputted by the user on the terminal.
  */
std::string IOHelper::readLine() {
  std::string str;
  std::getline(std::cin, str);
  return str;
}

void IOHelper::printEndLine() {
  std::cout << "\n";
}
