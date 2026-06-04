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
#include <firefly/log-entry.hpp>
#include <firefly/sinks/i-sink.hpp>
#include "firefly/log-color.hpp"
#include "firefly/sinks/csv-sink.hpp"
#include "firefly/sinks/json-sink.hpp"
#include "firefly/sinks/plain-text-sink.hpp"
#include <firefly/log-levels/i-log-level.hpp>
#include <firefly/log-levels/log-level-info.hpp>

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
  this->InitializeSinks();
  this->CreateAndOpenLogFile();
}

Logger::~Logger()
{
  this->CloseLogFile();
}

void Logger::InitializeSinks()
{
  this->sinks.emplace(".txt", std::make_unique<Sinks::PlainTextSink>());
  this->sinks.emplace(".csv", std::make_unique<Sinks::CsvSink>());
  this->sinks.emplace(".json", std::make_unique<Sinks::JsonSink>());
  this->sinks.emplace(".ndjson", std::make_unique<Sinks::NdjsonSink>());
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

void Logger::LogImpl(const LogLevels::ILogLevel& level, std::string formattedMessage)
{
  if (Logger::ShouldLogMessage(level)) {
    if (this->logFile.is_open()) {
      this->WriteToFile(level, formattedMessage);
    }
    std::string timestamp = this->GetTimestamp();
    std::string loggerName = std::format("[{}] ", this->name);
    std::string levelString = std::format("[{}]: ", level.GetName());
    std::cout << level.GetColor() << timestamp << loggerName << levelString << formattedMessage
              << std::endl;
    this->ResetColor();
  }
}

bool Logger::ShouldLogMessage(const LogLevels::ILogLevel& level) const
{
  if (this->debugEnabled) {
    return true;
  }
  return level.GetPriority() >= LogLevels::Info::priority;
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
void Logger::WriteToFile(const LogLevels::ILogLevel& level, std::string message)
{
  auto now = ClockSync();
  auto& entry = this->logCache[message];

  entry.intervalCount++;
  entry.totalCount++;

  // SIDE QUEST: This needs testing, i don't think it is working correctly
  if (std::chrono::duration_cast<std::chrono::seconds>(now.steadyTime - entry.lastLogged.steadyTime)
          .count() >= 1) {
    entry.message = message;
    entry.loggerName = this->name;
    entry.logLevelName = level.GetName();
    this->WriteLineToFile(entry);
    entry.lastLogged = now;
    entry.intervalCount = 0;
  }
  else {
    entry.message = message;
    entry.loggerName = this->name;
    entry.logLevelName = level.GetName();
    entry.lastLogged = now;
    entry.intervalCount += 1;
    this->WriteLineToFile(entry);
  }
}

void Logger::WriteLineToFile(LogEntry entry)
{
  std::filesystem::path path(this->fileName);
  std::string extension = path.extension().string();

  auto it = this->sinks.find(extension);

  if (it != this->sinks.end()) {
    this->logFile << it->second->Format(entry);
  }
  else {
    this->logFile << this->defaultSink->Format(entry);
  }
}
}  // namespace Firefly
