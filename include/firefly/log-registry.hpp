#pragma once
#include "firefly/logger.hpp"
#include <memory>

namespace Firefly {
class LogRegistry {
 public:
  FIREFLY_API static void registerLogger(
      const std::string& loggerName, bool enableDebugLogging = false
  );

  FIREFLY_API static void registerLogger(
      const std::string& loggerName, const std::string& outputFileName,
      bool enableDebugLogging = false
  );

  FIREFLY_API static std::shared_ptr<Logger>& getLogger(const std::string& loggerName);

 private:
  static std::unordered_map<std::string, std::shared_ptr<Logger>> registeredLoggers;
};
}  // namespace Firefly
