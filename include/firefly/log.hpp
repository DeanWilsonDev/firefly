#pragma once
#include <firefly/core.hpp>
#include <firefly/logger.hpp>
#include <firefly/log-registry.hpp>

#define FIREFLY_DEFAULT_LOGGER "Client"

namespace Firefly {

// Log Macros
#define LOG_TRACE(...) Firefly::LogRegistry::GetLogger(FIREFLY_DEFAULT_LOGGER)->Trace(__VA_ARGS__)
#define LOG_DEBUG(...) Firefly::LogRegistry::GetLogger(FIREFLY_DEFAULT_LOGGER)->Debug(__VA_ARGS__)
#define LOG_INFO(...) Firefly::LogRegistry::GetLogger(FIREFLY_DEFAULT_LOGGER)->Info(__VA_ARGS__)
#define LOG_WARNING(...) \
  Firefly::LogRegistry::GetLogger(FIREFLY_DEFAULT_LOGGER)->Warning(__VA_ARGS__)
#define LOG_ERROR(...) Firefly::LogRegistry::GetLogger(FIREFLY_DEFAULT_LOGGER)->Error(__VA_ARGS__)
#define LOG_FATAL(...) Firefly::LogRegistry::GetLogger(FIREFLY_DEFAULT_LOGGER)->Fatal(__VA_ARGS__)

}  // namespace Firefly
