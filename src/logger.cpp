//
// Created by Dean Wilson on 2023/9/27
//

#include <chrono>
#include <string>
#include <ostream>
#include <filesystem>
#include <firefly/clock-sync.hpp>
#include <firefly/time.hpp>
#include <firefly/logger.hpp>
#include <unordered_map>
#include <amanuensis.hpp>
#include "amanuensis/io/writer-options.hpp"
#include "amanuensis/io/writer.hpp"
#include "firefly/log-color.hpp"
#include <firefly/log-levels/i-log-level.hpp>

namespace Firefly {

Logger::Logger(const std::string name, bool debugEnabled)
{
  this->name = std::move(name);
  this->debugEnabled = debugEnabled;
}

Logger::Logger(const std::string name, const std::string& fileName, bool debugEnabled)
{
  this->name = std::move(name);
  this->debugEnabled = debugEnabled;
  this->fileName = fileName;
  this->CreateAndOpenLogFile();
}

Logger::~Logger()
{
  this->CloseLogFile();
}

void Logger::ResetColor()
{
  std::cout << LogColor::Reset;
}

std::string Logger::GetTimestamp()
{
  std::time_t now = std::time(nullptr);
  std::tm timeinfo = localtime_safe(now);

  char timestamp[80];
  std::strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S] ", &timeinfo);

  return std::string(timestamp);
}

void Logger::LogImpl(const LogLevel::ILogLevel& level, std::string formattedMessage)
{
  if (Logger::ShouldLogMessage(level)) {
    if (this->logFile.is_open()) {
      this->WriteToFile(level, formattedMessage);
    }
    std::string timestamp = this->GetTimestamp();
    std::string loggerName = "[" + this->name + "] ";
    std::string levelString = "[" + level.GetName() + "]: ";
    std::cout << level.GetColor() << timestamp << loggerName << levelString << formattedMessage
              << std::endl;
    this->ResetColor();
  }
}

bool Logger::ShouldLogMessage(const LogLevel::ILogLevel& level) const
{
  if (this->debugEnabled) {
    return true;
  }
  return level.GetPriority() >= LogLevel::Info{}.GetPriority();
}

bool Logger::EnableDebugging()
{
  return debugEnabled = true;
}

bool Logger::DisableDebugging()
{
  return debugEnabled = false;
}

bool Logger::GetDebugEnabled() const
{
  return debugEnabled;
}

void Logger::CreateAndOpenLogFile()
{
  try {
    std::filesystem::path logDir = "log";
    if (!std::filesystem::exists(logDir)) {
      std::filesystem::create_directory(logDir);
    }

    std::filesystem::path filePath = logDir / this->fileName;

    // Open the log file for writing
    this->logFile.open(filePath, std::ios::app);
    if (!this->logFile.is_open()) {
      throw std::runtime_error("Failed to open log file: " + filePath.string());
    }
    this->fileEnabled = true;
  }
  catch (const std::exception& e) {
    std::cerr << "Logger initialization error: " << e.what() << std::endl;
  }
}

void Logger::CloseLogFile()
{
  if (this->logFile.is_open()) {
    this->logFile.close();
    this->fileEnabled = false;
  }
}

// 1UP: A LogFileWriter Class would allow for some good cleanup in this file
void Logger::WriteToFile(const LogLevel::ILogLevel& level, std::string message)
{
  auto now = ClockSync();
  auto& entry = this->logCache[message];

  entry.intervalCount++;
  entry.totalCount++;

  if (std::chrono::duration_cast<std::chrono::seconds>(now.steadyTime - entry.lastLogged.steadyTime)
          .count() >= 1) {
    entry.message = message;
    entry.logLevel = level.GetName();
    this->WriteLineToFile(entry);
    entry.lastLogged = now;
    entry.intervalCount = 0;
  }
  else {
    // entry.message = message;
    // entry.logLevel = level.GetName();
    // entry.lastLogged = now;
    entry.intervalCount += 1;
    // this->WriteLineToFile(entry);
  }
}

void Logger::WriteLineToFile(LogEntry entry)
{
  // 1UP: Map to LogFormat for easy reference of file formats in the future
  // std::unordered_map<std::string, LogFormat> formatMap = {
  //     {".csv", LogFormat::CSV}, {".json", LogFormat::JSON}
  // };

  std::filesystem::path path(this->fileName);
  std::string extension = path.extension().string();

  auto it = this->handlers.find(extension);

  if (it != handlers.end()) {
    it->second(entry);
  }
  else {
    this->WriteLineToPlainTextFileHandler(entry);
  }
}

void Logger::WriteLineToCsvFileHandler(LogEntry entry)
{
  std::string timestamp = ClockSync::SystemTimeToString(entry.lastLogged.systemTime);

  this->logFile << timestamp << "," << entry.logLevel.GetName() << "," << entry.message << ","
                << std::to_string(entry.intervalCount) << "," << std::to_string(entry.totalCount)
                << std::endl;
}

void Logger::WriteLineToPlainTextFileHandler(LogEntry entry)
{
  std::string timestamp = "[" + ClockSync::SystemTimeToString(entry.lastLogged.systemTime) + "]";
  std::string loggerName = "[" + this->name + "] ";
  std::string levelString = "[" + entry.logLevel.GetName() + "]: ";

  this->logFile << timestamp << levelString << entry.message << std::endl;
}

struct JsonPayload {
  std::string_view timestamp;
  std::string_view logLevel;
  std::string_view message;
  int intervalCount;
  int totalCount;
};

AMANUENSIS_SERIALISABLE(JsonPayload, timestamp, logLevel, message, intervalCount, totalCount);

// TODO: This function doesn't return proper JSON yet. It needs to be comma delimitted and wrapped
// in an array
void Logger::WriteLineToJsonFileHandler(LogEntry entry)
{
  std::string timestamp = ClockSync::SystemTimeToString(entry.lastLogged.systemTime);

  JsonPayload payload = JsonPayload{
      .timestamp = timestamp,
      .logLevel = entry.logLevel.GetName(),
      .message = entry.message,
      .intervalCount = entry.intervalCount,
      .totalCount = entry.totalCount
  };

  Amanuensis::Value jsonValue = Amanuensis::ToJson(payload);

  std::string jsonText = Amanuensis::Writer::WriteToString(jsonValue);

  this->logFile << jsonText;
}

void Logger::WriteLineToNdjsonFileHandler(LogEntry entry)
{
  std::string timestamp = ClockSync::SystemTimeToString(entry.lastLogged.systemTime);

  JsonPayload payload = JsonPayload{
      .timestamp = timestamp,
      .logLevel = entry.logLevel.GetName(),
      .message = entry.message,
      .intervalCount = entry.intervalCount,
      .totalCount = entry.totalCount

  };

  Amanuensis::WriterOptions ndjsonOptions;
  ndjsonOptions.pretty = false;
  ndjsonOptions.trailingNewline = true;

  Amanuensis::Value jsonValue = Amanuensis::ToJson(payload);

  std::string jsonText = Amanuensis::Writer::WriteToString(jsonValue, ndjsonOptions);

  this->logFile << jsonText;
}

}  // namespace Firefly
