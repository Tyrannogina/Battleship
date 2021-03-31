#include "config_parser.h"
#include "game.h"
#include "IOHelper.h"

/**
 * Displays initial menu with game options and option for quitting.
 */
void displayMenu() {
  IOHelper::printEndLine();
  IOHelper::printText("Welcome to ",
                      IOHelper::BOLD,
                      IOHelper::FG_BLUE,
                      IOHelper::BG_DEFAULT,
                      false);
  IOHelper::printText("Battle",
                      IOHelper::BOLD,
                      IOHelper::FG_RED,
                      IOHelper::BG_DEFAULT,
                      false);
  IOHelper::printMenuText("ship!");
  IOHelper::printMenuText("What would you like to do?");
  IOHelper::printMenuText("1 - One player vs. computer");
  IOHelper::printMenuText("0 - Quit");
}

int main() {
  int selection;
  do {
    displayMenu();
    try {
      selection = std::stoi(IOHelper::readLine());
      switch (selection) {
        case 1: {
          ConfigParser configParser;
          Config config = configParser.parseConfig();
          Game game(false, true, config);
          break;
        }
        case 2:break;
        case 0:IOHelper::printMenuText("Goodbye");
          break;
        default:
          IOHelper::printMenuText(
              "That was not a valid menu option, please try again!");
      }
    } catch (...) {
      IOHelper::printMenuText(
          "An error has occurred. Exiting the program");
      selection = 0;
    }
  } while (selection != 0);
}