#include "firefly/sinks/csv-sink.hpp"
#include <firefly/clock-sync.hpp>
#include "firefly/log-entry.hpp"

namespace Firefly::Sinks {

std::string CsvSink::Format(LogEntry entry) const
{
  std::string timestamp = ClockSync::SystemTimeToString(entry.lastLogged.systemTime);

  return std::format(
      "{}, {}, {}, {}, {}, {}\n",
      timestamp,
      entry.loggerName,
      entry.logLevelName,
      entry.message,
      std::to_string(entry.intervalCount),
      std::to_string(entry.totalCount)
  );
}

}  // namespace Firefly::Sinks
