#include <firefly/log-factory.h>
#include <memory>

namespace Firefly::Logging {

static std::shared_ptr<Logger> createLogger(
    const std::string& loggerName, bool enableDebugLogging = false
)
{
  return std::make_shared<Logger>(loggerName, enableDebugLogging);
};

static std::shared_ptr<Logger> createLogger(
    const std::string& loggerName, const std::string& outputFileName,
    bool enableDebugLogging = false
)
{
  return std::make_shared<Logger>(loggerName, outputFileName, enableDebugLogging);
};

}  // namespace Firefly::Logging
