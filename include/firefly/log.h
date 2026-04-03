#pragma once

#include <firefly/logger.h>
#include <string>

namespace Firefly {
namespace Logging {

#ifdef FIREFLY_PLATFORM_MACOS
#define FIREFLY_API __attribute__((visibility("default")))
#elif FIREFLY_PLATFORM_WINDOWS
#define FIREFLY_API __declspec(dllexport)
#else
#define FIREFLY_API
#endif

#define BIT(x) (1 << x)

class Log {
 public:
  FIREFLY_API static void init(bool enableClientDebugLogging = true);
  FIREFLY_API static void init(std::string fileName, bool enableClientDebugLogging = true);
  FIREFLY_API inline static std::shared_ptr<Logger>& getCoreLogger() { return CoreLogger; };
  FIREFLY_API inline static std::shared_ptr<Logger>& getClientLogger() { return ClientLogger; }

 private:
  static std::shared_ptr<Logger> CoreLogger;
  static std::shared_ptr<Logger> ClientLogger;
};
}  // namespace Logging
}  // namespace Firefly

// Client Log Macros
#define LOG_TRACE(...) Firefly::Logging::Log::getClientLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...) Firefly::Logging::Log::getClientLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...) Firefly::Logging::Log::getClientLogger()->info(__VA_ARGS__)
#define LOG_WARNING(...) Firefly::Logging::Log::getClientLogger()->warning(__VA_ARGS__)
#define LOG_ERROR(...) Firefly::Logging::Log::getClientLogger()->error(__VA_ARGS__)
#define LOG_FATAL(...) Firefly::Logging::Log::getClientLogger()->fatal(__VA_ARGS__)

// Core Log Macros
#define LOG_CORE_TRACE(...) Firefly::Logging::Log::getCoreLogger()->trace(__VA_ARGS__)
#define LOG_CORE_DEBUG(...) Firefly::Logging::Log::getCoreLogger()->debug(__VA_ARGS__)
#define LOG_CORE_INFO(...) Firefly::Logging::Log::getCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_WARNING(...) Firefly::Logging::Log::getCoreLogger()->warning(__VA_ARGS__)
#define LOG_CORE_ERROR(...) Firefly::Logging::Log::getCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_FATAL(...) Firefly::Logging::Log::getCoreLogger()->fatal(__VA_ARGS__)
