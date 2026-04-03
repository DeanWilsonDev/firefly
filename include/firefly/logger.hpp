#pragma once

#include <firefly/core.hpp>
#include <firefly/clock-sync.hpp>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

namespace Firefly {

const int LOG_LEVEL_TRACE = 0;
const int LOG_LEVEL_DEBUG = 1;
const int LOG_LEVEL_INFO = 2;
const int LOG_LEVEL_WARNING = 3;
const int LOG_LEVEL_ERROR = 4;
const int LOG_LEVEL_FATAL = 5;
const int LOG_LEVEL_OFF = 6;

enum LogLevel : int {
  Trace = LOG_LEVEL_TRACE,
  Debug = LOG_LEVEL_DEBUG,
  Info = LOG_LEVEL_INFO,
  Warning = LOG_LEVEL_WARNING,
  Error = LOG_LEVEL_ERROR,
  Fatal = LOG_LEVEL_FATAL,
  Off = LOG_LEVEL_OFF
};

const int LOG_FORMAT_PLAIN_TEXT = 0;
const int LOG_FORMAT_CSV = 1;
const int LOG_FORMAT_JSON = 2;

enum LogFormat : int {
  PLAIN_TEXT = LOG_FORMAT_PLAIN_TEXT,
  CSV = LOG_FORMAT_CSV,
  JSON = LOG_FORMAT_JSON
};

struct LogEntry {
  ClockSync lastLogged;
  LogLevel logLevel;
  std::string message;
  int intervalCount;
  int totalCount;
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

  template <typename... Args>
  void Trace(const std::format_string<Args...> message, Args&&... args)
  {
    LogFormattedString(LogLevel::Trace, message, std::forward<Args>(args)...);
  }

  template <typename... Args>
  void Debug(const std::format_string<Args...> message, Args&&... args)
  {
    LogFormattedString(LogLevel::Debug, message, std::forward<Args>(args)...);
  }

  template <typename... Args>
  void Info(const std::format_string<Args...> message, Args&&... args)
  {
    LogFormattedString(LogLevel::Info, message, std::forward<Args>(args)...);
  }

  template <typename... Args>
  void Warning(const std::format_string<Args...> message, Args&&... args)
  {
    LogFormattedString(LogLevel::Warning, message, std::forward<Args>(args)...);
  }

  template <typename... Args>
  void Error(const std::format_string<Args...> message, Args&&... args)
  {
    LogFormattedString(LogLevel::Error, message, std::forward<Args>(args)...);
  }

  template <typename... Args>
  void Fatal(const std::format_string<Args...> message, Args&&... args)
  {
    LogFormattedString(LogLevel::Fatal, message, std::forward<Args>(args)...);
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

  [[nodiscard]] bool ShouldLogMessage(LogLevel level) const;
  [[nodiscard]] bool GetDebugEnabled() const;
  void CreateAndOpenLogFile();
  void CloseLogFile();
  void WriteToFile(LogLevel level, std::string message);
  void WriteLineToFile(LogEntry entry);
  void WriteLineToCsvFileHandler(LogEntry entry);
  void WriteLineToJsonFileHandler(LogEntry entry);
  void WriteLineToPlainTextFileHandler(LogEntry entry);
  void SetColor(LogLevel level);
  void ResetColor();
  std::string GetLevelString(LogLevel level);
  std::string GetTimestamp();

  std::unordered_map<std::string, std::function<void(LogEntry entry)>> handlers = {
      {".csv", [this](LogEntry entry) { this->WriteLineToCsvFileHandler(entry); }},
      {".json", [this](LogEntry entry) { this->WriteLineToJsonFileHandler(entry); }}
  };

  template <typename... Args>
  void LogFormattedString(LogLevel level, const std::format_string<Args...> format, Args&&... args)
  {
    std::string formattedString = std::format(format, std::forward<Args>(args)...);
    this->Log(level, formattedString);
  };

  void Log(LogLevel level, std::string formattedMessage);
};
}  // namespace Firefly
