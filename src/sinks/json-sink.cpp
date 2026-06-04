#include "json-payload.hpp"
#include <firefly/sinks/json-sink.hpp>
#include <firefly/clock-sync.hpp>
#include "firefly/log-entry.hpp"
#include <amanuensis.hpp>

namespace Firefly::Sinks {

std::string JsonSink::Format(LogEntry entry) const
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

  Amanuensis::WriterOptions jsonOptions;
  jsonOptions.pretty = true;
  jsonOptions.trailingNewline = false;

  Amanuensis::Value jsonValue = Amanuensis::ToJson(payload);
  std::string jsonString = Amanuensis::Writer::WriteToString(jsonValue, jsonOptions);

  return std::format("{},\n", jsonString);
}
}  // namespace Firefly::Sinks
