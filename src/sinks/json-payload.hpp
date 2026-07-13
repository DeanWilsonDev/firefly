#pragma once
#include <amanuensis.hpp>

namespace Firefly {

struct JsonPayload {
  std::string timestamp;
  std::string loggerName;
  std::string logLevel;
  std::string message;
  int intervalCount;
  int totalCount;
};

AMANUENSIS_SERIALISABLE(
    JsonPayload, timestamp, loggerName, logLevel, message, intervalCount, totalCount
);

}  // namespace Firefly
