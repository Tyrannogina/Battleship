#ifndef PRINTER_H
#define PRINTER_H

#include <string>

class IOHelper {
 public:
  IOHelper();

  static std::string const inline BOLD = "1";
  static std::string const inline REGULAR = "0";

  static std::string const inline FG_DEFAULT = "39";
  static std::string const inline FG_BLUE = "34";
  static std::string const inline FG_RED = "31";
  static std::string const inline FG_WHITE = "37";

  static std::string const inline BG_DEFAULT = "49";
  static std::string const inline BG_BLUE = "44";
  static std::string const inline BG_RED = "41";

  static void printMenuText(const std::string& str);

  static void printText(const std::string& str,
                        const std::string& boldness = REGULAR,
                        const std::string& foreground = FG_DEFAULT,
                        const std::string& background = BG_DEFAULT,
                        bool endl = false);
  static void printEndLine();
  static std::string readLine();
  static void printBoardLabelText(const std::string& str);
};

#endif //PRINTER_H

