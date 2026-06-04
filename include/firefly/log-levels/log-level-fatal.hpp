#pragma once

#include "firefly/log-levels/i-log-level.hpp"
#include "firefly/log-color.hpp"
#include <string_view>

namespace Firefly {
namespace LogLevels {

class Fatal : public ILogLevel {
 public:
  static constexpr std::string_view name{"FATAL"};
  static constexpr std::string_view color{LogColor::Magenta};
  static constexpr int priority{5};

  Fatal() = default;
  ~Fatal() = default;
  std::string_view GetColor() const override { return this->color; }
  std::string_view GetName() const override { return this->name; }
  int GetPriority() const override { return this->priority; }
};
}  // namespace LogLevels
}  // namespace Firefly
