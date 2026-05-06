# Firefly — Logging Library

A lightweight, header-friendly C++20 logging library designed for game development and tooling.
Firefly supports console output with color-coded log levels, file output in multiple formats,
and a global logger registry for multi-logger applications.

______________________________________________________________________

## Features

- **Color-coded console output** per log level (Trace, Debug, Info, Warning, Error, Fatal)
- **File logging** with support for `.txt`, `.csv`, and `.json` output formats
- **Log deduplication** — repeated messages within a 1-second window are coalesced
- **Format string support** via `std::format` (C++20)
- **Multiple named loggers** via `LogRegistry`
- **Convenience macros** for zero-boilerplate logging
- **Cross-platform** — works on Windows, macOS, and Linux

______________________________________________________________________

## Requirements

- C++20 or later
- CMake 3.25 or later
- A compiler with `std::format` support (GCC 13+, Clang 16+, MSVC 19.29+)

______________________________________________________________________

## Installation

### Option A — Git Submodule

```bash
git submodule add git@github.com:DeanWilsonDev/firefly.git external/firefly
git submodule update --init --recursive
```

In your project's `CMakeLists.txt`:

```cmake
add_subdirectory(external/firefly)
target_link_libraries(your-project PRIVATE firefly)
```

### Option B — Local Install via `find_package`

Build and install Firefly to your system (or a local prefix), then consume it
from any CMake project using `find_package`.

**1. Clone and install:**

```bash
git clone git@github.com:DeanWilsonDev/firefly.git
cd firefly
cmake -B build -DCMAKE_INSTALL_PREFIX=~/.local   # or /usr/local, or any prefix you prefer
cmake --build build
cmake --install build
```

**2. In your project's `CMakeLists.txt`:**

```cmake
find_package(firefly REQUIRED)
target_link_libraries(your-project PRIVATE Firefly::firefly)
```

**3. Configure your project, pointing CMake at the install prefix if needed:**

```bash
cmake -B build -DCMAKE_PREFIX_PATH=~/.local
cmake --build build
```

> The `CMAKE_PREFIX_PATH` step is only required if you installed to a non-standard
> prefix (i.e. anything other than `/usr` or `/usr/local`).
> `find_package` respects `SameMajorVersion` compatibility — a project requesting
> `firefly 1.x` will match any installed `1.y` release.

______________________________________________________________________

## Quick Start

### 1. Register a Logger

Before logging anything, register at least one logger with `LogRegistry`.
The name you register it under is used to retrieve it later.

```cpp
#include <firefly/log.hpp>
#include <firefly/log-registry.hpp>

int main()
{
    // Register a console-only logger
    Firefly::LogRegistry::RegisterLogger("Client");

    // Or register with a file output
    Firefly::LogRegistry::RegisterLogger("Client", "output.csv");

    // Enable debug-level logging (off by default)
    Firefly::LogRegistry::RegisterLogger("Client", true);
}
```

### 2. Log Messages Using Macros

Include `<firefly/log.hpp>` and use the convenience macros.
These always target the logger registered under the name `"Client"` (the default logger).

```cpp
#include <firefly/log.hpp>
#include <firefly/log-registry.hpp>

int main()
{
    Firefly::LogRegistry::RegisterLogger(FIREFLY_DEFAULT_LOGGER, "log.csv", true);

    LOG_TRACE("{} Testing Trace Logging", 1);
    LOG_DEBUG("{} Testing Debug Logging", 2);
    LOG_INFO("Player health: {}", 100);
    LOG_WARNING("Low memory: {} MB remaining", 512);
    LOG_ERROR("Failed to load asset: {}", "hero.png");
    LOG_FATAL("Unrecoverable error in subsystem: {}", "renderer");
}
```

### 3. Use the Logger Directly

You can also retrieve a logger from the registry and call methods on it directly:

```cpp
auto& logger = Firefly::LogRegistry::GetLogger("Client");
logger->Info("Engine initialized in {} ms", 42);
logger->Warning("Config file not found, using defaults");
```

______________________________________________________________________

## Log Levels

Firefly defines the following log levels, in ascending order of severity:

| Level | Macro | Color | Notes |
|-----------|-----------------|---------|--------------------------------------------|
| `Trace` | `LOG_TRACE` | Green | Fine-grained diagnostic info |
| `Debug` | `LOG_DEBUG` | Blue | Debug info; only shown when debug is enabled |
| `Info` | `LOG_INFO` | Cyan | General runtime information |
| `Warning` | `LOG_WARNING` | Yellow | Non-critical issues |
| `Error` | `LOG_ERROR` | Red | Recoverable errors |
| `Fatal` | `LOG_FATAL` | Magenta | Unrecoverable errors |
| `Off` | — | — | Disables logging entirely |

> **Note:** `Trace` and `Debug` messages are suppressed unless the logger was registered
> with `enableDebugLogging = true`.

______________________________________________________________________

## File Output Formats

When registering a logger with a file name, Firefly infers the output format from the
file extension.

| Extension | Format | Notes |
|-----------|------------|-----------------------------------------------|
| `.txt` | Plain text | Default fallback for any unrecognized extension |
| `.csv` | CSV | Columns: timestamp, level, message, interval count, total count |
| `.json` | JSON | Partially implemented — currently outputs CSV-style text |

### CSV Output Example

```
2025-09-27 14:32:01.042,INFO,Engine initialized in 42 ms,1,1
2025-09-27 14:32:01.043,WARNING,Config file not found,1,1
```

### Plain Text Output Example

```
[2025-09-27 14:32:01.042][INFO]: Engine initialized in 42 ms
[2025-09-27 14:32:01.043][WARNING]: Config file not found
```

Log files are written to a `log/` directory relative to the working directory.
The directory is created automatically if it does not exist.

______________________________________________________________________

## API Reference

### `Firefly::LogRegistry`

Static registry for managing named `Logger` instances.

```cpp
// Register a console-only logger
static void RegisterLogger(const std::string& loggerName, bool enableDebugLogging = false);

// Register a logger with file output
static void RegisterLogger(
    const std::string& loggerName,
    const std::string& outputFileName,
    bool enableDebugLogging = false
);

// Retrieve a registered logger by name
static std::shared_ptr<Logger>& GetLogger(const std::string& loggerName);
```

> Calling `GetLogger` with an unregistered name will throw `std::out_of_range`.

______________________________________________________________________

### `Firefly::Logger`

Core logging class. Supports formatted messages via `std::format`.

```cpp
// Log at a specific level with format string support
void Trace(std::format_string<Args...> message, Args&&... args);
void Debug(std::format_string<Args...> message, Args&&... args);
void Info(std::format_string<Args...> message, Args&&... args);
void Warning(std::format_string<Args...> message, Args&&... args);
void Error(std::format_string<Args...> message, Args&&... args);
void Fatal(std::format_string<Args...> message, Args&&... args);

// Toggle debug-level output at runtime
bool EnableDebugging();
bool DisableDebugging();
```

______________________________________________________________________

## Macros

Defined in `<firefly/log.hpp>`. All macros target the logger named `FIREFLY_DEFAULT_LOGGER`
(`"Client"` by default).

```cpp
#define FIREFLY_DEFAULT_LOGGER "Client"

#define LOG_TRACE(...)
#define LOG_DEBUG(...)
#define LOG_INFO(...)
#define LOG_WARNING(...)
#define LOG_ERROR(...)
#define LOG_FATAL(...)
```

To use a different default logger name, redefine `FIREFLY_DEFAULT_LOGGER` before including
`<firefly/log.hpp>`.

______________________________________________________________________

## Building from Source

```bash
git clone git@github.com:DeanWilsonDev/firefly.git
cd firefly
cmake -B build
cmake --build build
```

To run the test executable:

```bash
./build/bin/test_firefly
```

To disable building tests:

```bash
cmake -B build -DFIREFLY_BUILD_TESTS=OFF
cmake --build build
```

______________________________________________________________________

## Project Structure

```
firefly/
├── include/
│   └── firefly/
│       ├── core.hpp          # Platform macros (FIREFLY_API)
│       ├── log.hpp           # Convenience macros (LOG_INFO, etc.)
│       ├── log-registry.hpp  # LogRegistry class declaration
│       ├── logger.hpp        # Logger class declaration
│       ├── clock-sync.hpp    # Timestamp helper struct
│       └── time.hpp          # Thread-safe localtime wrapper
├── src/
│   ├── logger.cpp            # Logger implementation
│   └── log-registry.cpp      # LogRegistry implementation
├── test/
│   └── test_main.cpp         # Manual test / usage example
├── include/pch.hpp           # Precompiled header
└── CMakeLists.txt
```

______________________________________________________________________

## License

MIT License — see [LICENSE](LICENSE) for details.
