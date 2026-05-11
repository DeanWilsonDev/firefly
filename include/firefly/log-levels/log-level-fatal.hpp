#pragma once

#include "firefly/log-levels/i-log-level.hpp"
#include "firefly/log-color.hpp"
#include <string_view>

namespace Firefly {
namespace LogLevel {

class Fatal : public ILogLevel {
 public:
  Fatal();
  ~Fatal() = default;
  std::string_view GetColor() const override { return this->color; }
  std::string_view GetName() const override { return this->name; }
  int GetPriority() const override { return this->priority; }

 private:
  std::string_view name{"FATAL"};
  std::string_view color{LogColor::Magenta};
  int priority{5};
};

}  // namespace LogLevel
}  // namespace Firefly
