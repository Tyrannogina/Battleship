#ifndef PRINTER_H
#define PRINTER_H

enum Code {
  FG_RED = 31,
  FG_GREEN = 32,
  FG_BLUE = 34,
  FG_DEFAULT = 39,
  BG_RED = 41,
  BG_GREEN = 42,
  BG_BLUE = 44,
  BG_DEFAULT = 49
};

class Printer {
 public:
  Printer();
};

#endif

// std::cout << "Hello World!🌊 🌊 💥 🚢 ❔\n";
// std::cout << "\033[0;34mbold green text\033[0m\n";
// std::cout << "\033[4;31munderlined red text\033[0m\n";
// std::cout << "\033[1;34;107mbold magenta text\033[0m\n";
// std::cout << "\033[1;31mbold red text\033[0m\n";