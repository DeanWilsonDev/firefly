#pragma once
#include <firefly/core.hpp>
#include <firefly/logger.hpp>
#include <firefly/log-registry.hpp>
#include <firefly/log-levels.hpp>

#define FIREFLY_DEFAULT_LOGGER "Client"

namespace Firefly {

// Log Macros

#define LOG_TRACE(...)                                    \
  Firefly::LogRegistry::GetLogger(FIREFLY_DEFAULT_LOGGER) \
      ->Log<Firefly::LogLevels::Trace>(__VA_ARGS__)

#define LOG_DEBUG(...)                                    \
  Firefly::LogRegistry::GetLogger(FIREFLY_DEFAULT_LOGGER) \
      ->Log<Firefly::LogLevels::Debug>(__VA_ARGS__)

#define LOG_INFO(...)                                     \
  Firefly::LogRegistry::GetLogger(FIREFLY_DEFAULT_LOGGER) \
      ->Log<Firefly::LogLevels::Info>(__VA_ARGS__)

#define LOG_WARNING(...)                                  \
  Firefly::LogRegistry::GetLogger(FIREFLY_DEFAULT_LOGGER) \
      ->Log<Firefly::LogLevels::Warning>(__VA_ARGS__)

#define LOG_ERROR(...)                                    \
  Firefly::LogRegistry::GetLogger(FIREFLY_DEFAULT_LOGGER) \
      ->Log<Firefly::LogLevels::Error>(__VA_ARGS__)

#define LOG_FATAL(...)                                    \
  Firefly::LogRegistry::GetLogger(FIREFLY_DEFAULT_LOGGER) \
      ->Log<Firefly::LogLevels::Fatal>(__VA_ARGS__)

}  // namespace Firefly
