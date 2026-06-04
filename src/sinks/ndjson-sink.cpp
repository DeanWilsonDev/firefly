#include "json-payload.hpp"
#include <firefly/sinks/ndjson-sink.hpp>
#include <firefly/clock-sync.hpp>
#include "firefly/log-entry.hpp"
#include <amanuensis.hpp>

namespace Firefly::Sinks {

std::string NdjsonSink::Format(LogEntry entry) const
{
  std::string timestamp = ClockSync::SystemTimeToString(entry.lastLogged.systemTime);

  JsonPayload payload = JsonPayload{
      .timestamp = timestamp,
      .loggerName = entry.loggerName,
      .logLevel = entry.logLevelName,
      .message = entry.message,
      .intervalCount = entry.intervalCount,
      .totalCount = entry.totalCount
  };

  Amanuensis::WriterOptions ndjsonOptions;
  ndjsonOptions.pretty = false;
  ndjsonOptions.trailingNewline = true;

  Amanuensis::Value jsonValue = Amanuensis::ToJson(payload);

  return Amanuensis::Writer::WriteToString(jsonValue, ndjsonOptions);
}
}  // namespace Firefly::Sinks
