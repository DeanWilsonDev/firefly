#include <firefly/log.hpp>
#include <firefly/log-registry.hpp>
#include "firefly/log-color.hpp"

using namespace Firefly;

int main()
{
  Firefly::LogRegistry::RegisterLogger(FIREFLY_DEFAULT_LOGGER, "log.json", true);

  LOG_TRACE("{} Testing Trace Logging", 1);
  LOG_DEBUG("{} Testing Debug Logging", 2);
  LOG_INFO("{} Testing Info Logging", 3);
  LOG_WARNING("{} Testing Warning Logging", 4);
  LOG_ERROR("{} Testing Error Logging", 5);
  LOG_FATAL("{} Testing Fatal Logging", 6);

  for (int i = 0; i < 10; ++i) {
    LOG_DEBUG("Testing Update Logging");
  }

  PRINT("Hello", Color::Green);
  PRINT(" World {}\n", Color::Blue, "!");

  PRINT_LINE("FOO BAR", Color::Red);
  PRINT("New Line!\n", Color::Magenta);
}
