#include <firefly/log.hpp>
#include <firefly/log-registry.hpp>
#include "firefly/log-color.hpp"

#include <chrono>
#include <filesystem>
#include <string>
#include <system_error>

using namespace Firefly;

namespace {
std::filesystem::path CreateScratchLogDirectory()
{
  const auto temporaryDirectory = std::filesystem::temp_directory_path();
  auto uniqueSuffix = std::chrono::steady_clock::now().time_since_epoch().count();
  while (true) {
    const auto scratchDirectory =
        temporaryDirectory / ("firefly-test-" + std::to_string(uniqueSuffix));
    if (std::filesystem::create_directory(scratchDirectory)) {
      return scratchDirectory;
    }
    ++uniqueSuffix;
  }
}
}

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

  std::error_code logFileSizeError;
  const auto logFileSize =
      std::filesystem::file_size(scratchLogDirectory / "log" / "log.json", logFileSizeError);
  const bool logFileWasWritten = !logFileSizeError && logFileSize > 0;
  std::filesystem::remove_all(scratchLogDirectory);
  return logFileWasWritten ? 0 : 1;
}
