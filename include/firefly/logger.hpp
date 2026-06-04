#pragma once

#include <firefly/core.hpp>
#include <firefly/clock-sync.hpp>
#include <firefly/log-entry.hpp>
#include <firefly/sinks.hpp>
#include <fstream>
#include <format>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include "firefly/log-levels/i-log-level.hpp"
#include "firefly/sinks/i-sink.hpp"
#include "firefly/sinks/plain-text-sink.hpp"

namespace Firefly {

class Logger {
 public:
  Logger(const std::string name, bool debugEnabled = false);
  Logger(const std::string name, const std::string& fileName, bool debugEnabled = false);
  ~Logger();

  Logger& operator=(const Logger& other)
  {
    if (this == &other) {
      return *this;
    }

    if (this->logFile.is_open()) {
      this->logFile.close();
    }

    std::string newFileName = other.logFile.rdbuf()->getloc().name();
    this->logFile.open(newFileName, std::ios::out | std::ios::app);

    return *this;
  }

  template <typename TLogLevel, typename... Args>
  void Log(const std::format_string<Args...> message, Args&&... args)
  {
    static_assert(
        std::is_base_of_v<LogLevels::ILogLevel, TLogLevel>, "TLogLevel must implement ILogLevel"
    );
    std::string formattedString = std::format(message, std::forward<Args>(args)...);
    this->LogImpl(TLogLevel{}, formattedString);
  }

  bool EnableDebugging();
  bool DisableDebugging();

 private:
  std::string name;
  bool debugEnabled;
  std::ofstream logFile;
  bool fileEnabled;
  std::string fileName;
  std::unordered_map<std::string, LogEntry> logCache;
  std::unique_ptr<Sinks::ISink> defaultSink = std::make_unique<Sinks::PlainTextSink>();

  [[nodiscard]] bool ShouldLogMessage(const LogLevels::ILogLevel& level) const;
  [[nodiscard]] bool GetDebugEnabled() const;
  void InitializeSinks();
  void CreateAndOpenLogFile();
  void CloseLogFile();
  void WriteToFile(const LogLevels::ILogLevel& level, std::string message);
  void WriteLineToFile(LogEntry entry);
  void ResetColor();
  std::string GetTimestamp();

  std::unordered_map<std::string, std::unique_ptr<Sinks::ISink>> sinks;

  void LogImpl(const LogLevels::ILogLevel& level, std::string formattedMessage);
};
}  // namespace Firefly
