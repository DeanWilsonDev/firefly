#include <firefly/log.h>
#include <firefly/logger.h>

int main()
{
  Firefly::Logging::Log::init("log.csv", true);
  LOG_CORE_TRACE("{} Testing Internal Trace Logging", 1);
  LOG_CORE_DEBUG("{} Testing Internal Debug Logging", 2);
  LOG_CORE_INFO("{} Testing Internal Info Logging", 3);
  LOG_CORE_WARNING("{} Testing Internal Warning Logging", 4);
  LOG_CORE_ERROR("{} Testing Internal Error Logging", 5);
  LOG_CORE_FATAL("{} Testing Internal Fatal Logging", 6);

  LOG_TRACE("{} Testing Trace Logging", 7);
  LOG_DEBUG("{} Testing Debug Logging", 8);
  LOG_INFO("{} Testing Info Logging", 9);
  LOG_INFO("{} Testing Info Logging", 9);
  LOG_INFO("{} Testing Info Logging", 9);
  LOG_INFO("{} Testing Info Logging", 9);
  LOG_WARNING("{} Testing Warning Logging", 10);
  LOG_ERROR("{} Testing Error Logging", 11);
  LOG_FATAL("{} Testing Fatal Logging", 12);
}
