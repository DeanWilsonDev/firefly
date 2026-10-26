#include <firefly/log.hpp>
#include <firefly/log-registry.hpp>
#include "firefly/log-color.hpp"

#include <chrono>
#include <filesystem>
#include <string>

using namespace Firefly;

namespace {
std::filesystem::path CreateScratchLogDirectory()
{
  const auto uniqueSuffix = std::to_string(
      std::chrono::steady_clock::now().time_since_epoch().count()
  );
  const auto scratchDirectory =
      std::filesystem::temp_directory_path() / ("firefly-test-" + uniqueSuffix);
  std::filesystem::create_directories(scratchDirectory);
  return scratchDirectory;
}
}  // namespace

int main()
{
  const auto originalWorkingDirectory = std::filesystem::current_path();
  const auto scratchLogDirectory = CreateScratchLogDirectory();
  std::filesystem::current_path(scratchLogDirectory);

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

  PRINT(Color::Green, "Hello");
  PRINT(Color::Blue, " World {}\n", "!");

  PRINT_LINE(Color::Red, "FOO BAR");
  PRINT("New Line!\n");

  Firefly::LogRegistry::GetLogger(FIREFLY_DEFAULT_LOGGER).reset();
  std::filesystem::current_path(originalWorkingDirectory);

  const bool logFileWasWritten =
      std::filesystem::file_size(scratchLogDirectory / "log" / "log.json") > 0;
  std::filesystem::remove_all(scratchLogDirectory);
  return logFileWasWritten ? 0 : 1;
}
