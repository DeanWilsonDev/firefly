#pragma once
#include <ctime>

namespace Firefly {

inline std::tm localtime_safe(std::time_t time)
{
  std::tm tm{};
#if defined(_WIN32)
  localtime_s(&tm, &time);
#else
  localtime_r(&time, &tm);
#endif
  return tm;
}
}  // namespace Firefly
