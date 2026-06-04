#pragma once

#include <string_view>

namespace Firefly {
namespace LogLevels {
class ILogLevel {
 public:
  virtual ~ILogLevel() = default;
  virtual std::string_view GetColor() const = 0;
  virtual std::string_view GetName() const = 0;
  virtual int GetPriority() const = 0;
};
}  // namespace LogLevel
}  // namespace Firefly
