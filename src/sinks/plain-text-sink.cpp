#include <firefly/sinks/plain-text-sink.hpp>
#include <firefly/clock-sync.hpp>
#include "firefly/log-entry.hpp"

namespace Firefly::Sinks {

std::string PlainTextSink::Format(LogEntry entry) const
{
  std::string timestamp = ClockSync::SystemTimeToString(entry.lastLogged.systemTime);

  return std::format(
      "[{}] [{}] [{}]: {}\n", entry.loggerName, timestamp, entry.logLevelName, entry.message
  );
}

}  // namespace Firefly::Sinks
