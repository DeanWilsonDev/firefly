#pragma once

#ifdef FIREFLY_PLATFORM_MACOS
#define FIREFLY_API __attribute__((visibility("default")))
#elif FIREFLY_PLATFORM_WINDOWS
#define FIREFLY_API __declspec(dllexport)
#else
#define FIREFLY_API
#endif
