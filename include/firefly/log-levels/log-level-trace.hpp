#pragma once

#include "firefly/log-levels/i-log-level.hpp"
#include "firefly/log-color.hpp"
#include <string_view>

namespace Firefly {
namespace LogLevels {

class Trace : public ILogLevel {
 public:
  static constexpr std::string_view name{"TRACE"};
  static constexpr std::string_view color{LogColor::Green};
  static constexpr int priority{0};

  Trace() = default;
  ~Trace() = default;
  std::string_view GetColor() const override { return this->color; }
  std::string_view GetName() const override { return this->name; }
  int GetPriority() const override { return this->priority; }
};
}  // namespace LogLevels
}  // namespace Firefly
