#pragma once
#include "logger.h"
#include <memory>

namespace Firefly {
namespace Logging {

class LogFactory {
 public:
  static std::shared_ptr<Logger> createLogger(
      const std::string& loggerName, bool enableDebugLogging = false
  );

  static std::shared_ptr<Logger> createLogger(
      const std::string& loggerName, const std::string& outputFileName,
      bool enableDebugLogging = false
  );
};
}  // namespace Logging
}  // namespace Firefly
