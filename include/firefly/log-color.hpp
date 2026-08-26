#pragma once
#include <string_view>

namespace Firefly {
constexpr std::string_view LogColorReset = "\033[0m";
constexpr std::string_view LogColorGreen = "\033[0;32m";
constexpr std::string_view LogColorBlue = "\033[0;34m";
constexpr std::string_view LogColorCyan = "\033[0;36m";
constexpr std::string_view LogColorYellow = "\033[0;33m";
constexpr std::string_view LogColorRed = "\033[0;31m";
constexpr std::string_view LogColorMagenta = "\033[0;35m";

enum Color {
  Reset,
  Green,
  Blue,
  Cyan,
  Yellow,
  Red,
  Magenta,
};

inline std::string_view MapToLogColor(Color color)
{
  switch (color) {
    case Reset:
      return LogColorReset;
    case Green:
      return LogColorGreen;
    case Blue:
      return LogColorBlue;
    case Cyan:
      return LogColorCyan;
    case Yellow:
      return LogColorYellow;
    case Red:
      return LogColorRed;
    case Magenta:
      return LogColorMagenta;
    default:
      return LogColorReset;
  }
}

}  // namespace Firefly
