///
/// Created by Dean Wilson 4th June 2026
///

#pragma once

#include "firefly/sinks/i-sink.hpp"
namespace Firefly {

struct LogEntry;

namespace Sinks {

class NdjsonSink : public ISink {
 public:
  ~NdjsonSink() = default;

  void WriteToFile(LogEntry entry) const override;
};
}  // namespace Sinks
}  // namespace Firefly
