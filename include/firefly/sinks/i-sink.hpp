///
/// Created by Dean Wilson 4th June 2026
///

#pragma once
#include <string>

namespace Firefly {
struct LogEntry;

namespace Sinks {
class ISink {
 public:
  ~ISink() = default;

  virtual std::string Format(LogEntry entry) const = 0;
};
}  // namespace Sinks
}  // namespace Firefly
