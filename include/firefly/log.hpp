#pragma once
#include <firefly/core.hpp>
#include <firefly/logger.hpp>
#include <firefly/log-registry.hpp>
#include <string>

#define FIREFLY_DEFAULT_LOGGER "Client"

namespace Firefly {

// Log Macros
#define LOG_TRACE(...) Firefly::LogRegistry::getLogger(FIREFLY_DEFAULT_LOGGER)->trace(__VA_ARGS__)
#define LOG_DEBUG(...) Firefly::LogRegistry::getLogger(FIREFLY_DEFAULT_LOGGER)->debug(__VA_ARGS__)
#define LOG_INFO(...) Firefly::LogRegistry::getLogger(FIREFLY_DEFAULT_LOGGER)->info(__VA_ARGS__)
#define LOG_WARNING(...) \
  Firefly::LogRegistry::getLogger(FIREFLY_DEFAULT_LOGGER)->warning(__VA_ARGS__)
#define LOG_ERROR(...) Firefly::LogRegistry::getLogger(FIREFLY_DEFAULT_LOGGER)->error(__VA_ARGS__)
#define LOG_FATAL(...) Firefly::LogRegistry::getLogger(FIREFLY_DEFAULT_LOGGER)->fatal(__VA_ARGS__)

}  // namespace Firefly
