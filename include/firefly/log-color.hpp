#pragma once
#include <string_view>

namespace Firefly {
namespace LogColor {
constexpr std::string_view Green = "\033[0;32m";
constexpr std::string_view Blue = "\033[0;34m";
constexpr std::string_view Cyan = "\033[0;36m";
constexpr std::string_view Yellow = "\033[0;33m";
constexpr std::string_view Red = "\033[0;31m";
constexpr std::string_view Magenta = "\033[0;35m";
constexpr std::string_view Reset = "\033[0m";

}  // namespace LogColor
}  // namespace Firefly
