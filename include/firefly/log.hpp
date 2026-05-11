#pragma once
#include <firefly/core.hpp>
#include <firefly/logger.hpp>
#include <firefly/log-registry.hpp>

#define FIREFLY_DEFAULT_LOGGER "Client"

namespace Firefly {

// Log Macros

#define LOG_TRACE(...)                                    \
  Firefly::LogRegistry::GetLogger(FIREFLY_DEFAULT_LOGGER) \
      ->Log<Firefly::LogLevel::Trace>(__VA_ARGS__)

#define LOG_DEBUG(...)                                    \
  Firefly::LogRegistry::GetLogger(FIREFLY_DEFAULT_LOGGER) \
      ->Log<Firefly::LogLevel::Debug>(__VA_ARGS__)

#define LOG_INFO(...) \
  Firefly::LogRegistry::GetLogger(FIREFLY_DEFAULT_LOGGER)->Log<Firefly::LogLevel::Info>(__VA_ARGS__)

#define LOG_WARNING(...)                                  \
  Firefly::LogRegistry::GetLogger(FIREFLY_DEFAULT_LOGGER) \
      ->Log<Firefly::LogLevel::Warning>(__VA_ARGS__)

#define LOG_ERROR(...)                                    \
  Firefly::LogRegistry::GetLogger(FIREFLY_DEFAULT_LOGGER) \
      ->Log<Firefly::LogLevel::Error>(__VA_ARGS__)

#define LOG_FATAL(...)                                    \
  Firefly::LogRegistry::GetLogger(FIREFLY_DEFAULT_LOGGER) \
      ->Log<Firefly::LogLevel::Fatal>(__VA_ARGS__)

}  // namespace Firefly
