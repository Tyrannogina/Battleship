#include <iostream>
#include "IOHelper.h"

IOHelper::IOHelper() = default;

/**
 * Prints standard menu text for the game: bold blue foreground on default
 * background, and ends the line.
 * @param str
 */
void IOHelper::printMenuText(const std::string& str) {
  printText(str, BOLD, FG_BLUE, BG_DEFAULT, true);
}

void IOHelper::printContrastMenuText(const std::string& str) {
  printText(str, BOLD, FG_RED, BG_DEFAULT, true);
}

/**
 * Prints board label text: bold white foreground on red background.
 * @param str
 */
void IOHelper::printBoardLabelText(const std::string& str) {
  printText(str, BOLD, FG_WHITE, BG_RED, false);
}

/**
 * Prints text in colours and weight according to the parameters specified.
 * If the boolean endl is true, calls printEndLine()
 * @param str
 * @param boldness
 * @param foreground
 * @param background
 * @param endl
 */
void IOHelper::printText(const std::string& str,
                         const std::string& boldness,
                         const std::string& foreground,
                         const std::string& background,
                         bool endl) {
  std::cout << "\033[" << boldness << ";" << background << ";" << foreground
            << "m";
  std::cout << str;
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

/**
 * Prints a new line and resets text styling to default values.
 */
void IOHelper::printEndLine() {
  std::cout << "\033[" << REGULAR << "m\n";
}
