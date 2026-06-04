///
/// Created by Dean Wilson 4th June 2026
///

#pragma once
#include "firefly/clock-sync.hpp"
#include "firefly/log-levels/i-log-level.hpp"

namespace Firefly {
struct LogEntry {
  ClockSync lastLogged;
  std::string logLevelName;
  std::string logLevelColor;
  std::string message;
  int intervalCount;
  int totalCount;
};
}  // namespace Firefly
