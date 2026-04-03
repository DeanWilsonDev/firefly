#pragma once

#include <firefly/common/clock-sync.h>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

namespace Firefly {
namespace Logging {

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
  Logger(std::string name, bool debugEnabled = false);
  Logger(std::string name, std::string fileName, bool debugEnabled = false);
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
  void trace(const std::format_string<Args...> message, Args&&... args)
  {
    logFormattedString(LogLevel::Trace, message, std::forward<Args>(args)...);
  }

  template <typename... Args>
  void debug(const std::format_string<Args...> message, Args&&... args)
  {
    logFormattedString(LogLevel::Debug, message, std::forward<Args>(args)...);
  }

  template <typename... Args>
  void info(const std::format_string<Args...> message, Args&&... args)
  {
    logFormattedString(LogLevel::Info, message, std::forward<Args>(args)...);
  }

  template <typename... Args>
  void warning(const std::format_string<Args...> message, Args&&... args)
  {
    logFormattedString(LogLevel::Warning, message, std::forward<Args>(args)...);
  }

  template <typename... Args>
  void error(const std::format_string<Args...> message, Args&&... args)
  {
    logFormattedString(LogLevel::Error, message, std::forward<Args>(args)...);
  }

  template <typename... Args>
  void fatal(const std::format_string<Args...> message, Args&&... args)
  {
    logFormattedString(LogLevel::Fatal, message, std::forward<Args>(args)...);
  }

  bool enableDebugging();
  bool disableDebugging();

 private:
  std::string name;
  bool debugEnabled;
  std::ofstream logFile;
  bool fileEnabled;
  std::string fileName;
  std::unordered_map<std::string, LogEntry> logCache;

  [[nodiscard]] bool shouldLogMessage(LogLevel level) const;
  [[nodiscard]] bool getDebugEnabled() const;
  void createAndOpenLogFile();
  void closeLogFile();
  void writeToFile(LogLevel level, std::string message);
  void writeLineToFile(LogEntry entry);
  void writeLineToCsvFileHandler(LogEntry entry);
  void writeLineToJsonFileHandler(LogEntry entry);
  void writeLineToPlainTextFileHandler(LogEntry entry);
  void setColor(LogLevel level);
  void resetColor();
  std::string getLevelString(LogLevel level);
  std::string getTimestamp();

  std::unordered_map<std::string, std::function<void(LogEntry entry)>> handlers = {
      {".csv", [this](LogEntry entry) { this->writeLineToCsvFileHandler(entry); }},
      {".json", [this](LogEntry entry) { this->writeLineToJsonFileHandler(entry); }}
  };

  template <typename... Args>
  void logFormattedString(LogLevel level, const std::format_string<Args...> format, Args&&... args)
  {
    std::string formattedString = std::format(format, std::forward<Args>(args)...);
    this->log(level, formattedString);
  };

  void log(LogLevel level, std::string formattedMessage);
};
}  // namespace Logging
}  // namespace Firefly
