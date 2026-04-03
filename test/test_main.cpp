#include <firefly/log.hpp>
#include <firefly/log-registry.hpp>

using namespace Firefly;

int main()
{
  Firefly::LogRegistry::registerLogger(FIREFLY_DEFAULT_LOGGER, "log.csv", true);

  LOG_TRACE("{} Testing Trace Logging", 1);
  LOG_DEBUG("{} Testing Debug Logging", 2);
  LOG_INFO("{} Testing Info Logging", 3);
  LOG_WARNING("{} Testing Warning Logging", 4);
  LOG_ERROR("{} Testing Error Logging", 5);
  LOG_FATAL("{} Testing Fatal Logging", 6);
}
