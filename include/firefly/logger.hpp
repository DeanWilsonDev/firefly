#pragma once

#include <firefly/core.hpp>
#include <firefly/clock-sync.hpp>
#include <firefly/log-entry.hpp>
#include <fstream>
#include <format>
#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include "firefly/log-levels/i-log-level.hpp"

namespace Firefly {

const int LOG_FORMAT_PLAIN_TEXT = 0;
const int LOG_FORMAT_CSV = 1;
const int LOG_FORMAT_JSON = 2;
const int LOG_FORMAT_NDJSON = 3;

// 1UP: These should be promoted to classes with their own writing/formating functions
enum LogFormat : int {
  PLAIN_TEXT = LOG_FORMAT_PLAIN_TEXT,
  CSV = LOG_FORMAT_CSV,
  JSON = LOG_FORMAT_JSON,
  NDJSON = LOG_FORMAT_JSON,
};

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

  [[nodiscard]] bool ShouldLogMessage(const LogLevels::ILogLevel& level) const;
  [[nodiscard]] bool GetDebugEnabled() const;
  void CreateAndOpenLogFile();
  void CloseLogFile();
  void WriteToFile(const LogLevels::ILogLevel& level, std::string message);
  void WriteLineToFile(LogEntry entry);
  void WriteLineToCsvFileHandler(LogEntry entry);
  void WriteLineToJsonFileHandler(LogEntry entry);
  void WriteLineToNdjsonFileHandler(LogEntry entry);
  void WriteLineToPlainTextFileHandler(LogEntry entry);
  void ResetColor();
  std::string GetTimestamp();

  std::unordered_map<std::string, std::function<void(LogEntry entry)>> handlers = {
      {".csv", [this](LogEntry entry) { this->WriteLineToCsvFileHandler(entry); }},
      {".json", [this](LogEntry entry) { this->WriteLineToJsonFileHandler(entry); }},
      {".ndjson", [this](LogEntry entry) { this->WriteLineToNdjsonFileHandler(entry); }}
  };

  void LogImpl(const LogLevels::ILogLevel& level, std::string formattedMessage);
};
}  // namespace Firefly
