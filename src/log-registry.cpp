#include <firefly/log-registry.hpp>
#include <firefly/logger.hpp>
#include <memory>
#include <unordered_map>

namespace Firefly {

std::unordered_map<std::string, std::shared_ptr<Logger>> LogRegistry::registeredLoggers;

void LogRegistry::RegisterLogger(const std::string& loggerName, bool enableDebugLogging)
{
  registeredLoggers[loggerName] = std::make_shared<Logger>(loggerName, enableDebugLogging);
};

void LogRegistry::RegisterLogger(
    const std::string& loggerName, const std::string& outputFileName, bool enableDebugLogging
)
{
  registeredLoggers[loggerName] =
      std::make_shared<Logger>(loggerName, outputFileName, enableDebugLogging);
};

std::shared_ptr<Logger>& LogRegistry::GetLogger(const std::string& loggerName)
{
  return registeredLoggers.at(loggerName);
}

}  // namespace Firefly
