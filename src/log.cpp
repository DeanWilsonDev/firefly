//
// Created by Dean Wilson on 2025/01/20
//

#include <firefly/log.h>

namespace Firefly {
namespace Logging {

std::shared_ptr<Logger> Log::CoreLogger;
std::shared_ptr<Logger> Log::ClientLogger;

void Log::init(bool enableClientDebugLogging)
{
  CoreLogger = std::make_shared<Logger>("Core", true);
  ClientLogger = std::make_shared<Logger>("Client", enableClientDebugLogging);
}

void Log::init(std::string fileName, bool enableClientDebugLogging)
{
  CoreLogger = std::make_shared<Logger>("Core", fileName, true);
  ClientLogger = std::make_shared<Logger>("Client", fileName, enableClientDebugLogging);
}

}  // namespace Logging
}  // namespace Firefly
