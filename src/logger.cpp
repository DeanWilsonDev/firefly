//
// Created by Dean Wilson on 2023/9/27
//

#include <chrono>
#include <filesystem>
#include <firefly/common/clock-sync.h>
#include <firefly/common/time.h>
#include <firefly/logger.h>
#include <unordered_map>

namespace Firefly {
namespace Logging {

Logger::Logger(std::string name, bool debugEnabled)
{
  this->name = std::move(name);
  this->debugEnabled = debugEnabled;
}

Logger::Logger(std::string name, std::string fileName, bool debugEnabled)
{
  this->name = std::move(name);
  this->debugEnabled = debugEnabled;
  this->fileName = fileName;
  this->createAndOpenLogFile();
}

Logger::~Logger()
{
  this->closeLogFile();
}

void Logger::setColor(LogLevel level)
{
  switch (level) {
    case LogLevel::Trace:
      std::cout << "\033[0;32m";  // Green
      break;
    case LogLevel::Debug:
      std::cout << "\033[0;34m";  // Blue
      break;
    case LogLevel::Info:
      std::cout << "\033[0;36m";  // Grey [Normal]
      break;
    case LogLevel::Warning:
      std::cout << "\033[0;33m";  // Yellow
      break;
    case LogLevel::Error:
      std::cout << "\033[0;31m";  // Red
      break;
    case LogLevel::Fatal:
      std::cout << "\033[0;35m";  // Magenta
      break;
    case LogLevel::Off:
      std::cout << "\033[0m";  // Reset
      break;
    default:
      std::cout << "\033[0m";
      break;
  }
}

void Logger::resetColor()
{
  std::cout << "\033[0m";
}

std::string Logger::getLevelString(LogLevel level)
{
  switch (level) {
    // TODO: Add Tracing?
    case LogLevel::Trace:
      return "TRACE";
    case LogLevel::Debug:
      return "DEBUG";
    case LogLevel::Info:
      return "INFO";
    case LogLevel::Warning:
      return "WARNING";
    case LogLevel::Error:
      return "ERROR";
    case LogLevel::Fatal:
      return "FATAL";
    case LogLevel::Off:
      return "OFF";
    default:
      return "UNKNOWN";
  }
}

std::string Logger::getTimestamp()
{
  std::time_t now = std::time(nullptr);
  std::tm timeinfo = localtime_safe(now);

  char timestamp[80];
  std::strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S] ", &timeinfo);

  return std::string(timestamp);
}

void Logger::log(LogLevel level, std::string formattedMessage)
{
  if (Logger::shouldLogMessage(level)) {
    if (this->logFile.is_open()) {
      this->writeToFile(level, formattedMessage);
    }

    std::string timestamp = this->getTimestamp();
    std::string loggerName = "[" + this->name + "] ";
    std::string levelString = "[" + this->getLevelString(level) + "]: ";
    this->setColor(level);
    std::cout << timestamp << loggerName << levelString << formattedMessage << std::endl;
    this->resetColor();
  }
}

bool Logger::shouldLogMessage(LogLevel level) const
{
  if (this->debugEnabled) {
    return true;
  }
  return level >= LogLevel::Info;
}

bool Logger::enableDebugging()
{
  return debugEnabled = true;
}

bool Logger::disableDebugging()
{
  return debugEnabled = false;
}

bool Logger::getDebugEnabled() const
{
  return debugEnabled;
}

void Logger::createAndOpenLogFile()
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

void Logger::closeLogFile()
{
  if (this->logFile.is_open()) {
    this->logFile.close();
    this->fileEnabled = false;
  }
}

// 1UP: A LogFileWriter Class would allow for some good cleanup in this file
void Logger::writeToFile(LogLevel level, std::string message)
{
  auto now = ClockSync();
  auto& entry = this->logCache[message];

  entry.intervalCount++;
  entry.totalCount++;

  if (std::chrono::duration_cast<std::chrono::seconds>(now.steadyTime - entry.lastLogged.steadyTime)
          .count() >= 1) {
    entry.message = message;
    entry.logLevel = level;
    this->writeLineToFile(entry);
    entry.lastLogged = now;
    entry.intervalCount = 0;
  }
  else {
    entry.message = message;
    entry.logLevel = level;
    entry.lastLogged = now;
    entry.intervalCount = 1;
    this->writeLineToFile(entry);
  }
}

void Logger::writeLineToFile(LogEntry entry)
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
    this->writeLineToPlainTextFileHandler(entry);
  }
}

void Logger::writeLineToCsvFileHandler(LogEntry entry)
{
  std::string timestamp = ClockSync::systemTimeToString(entry.lastLogged.systemTime);

  this->logFile << timestamp << "," << this->getLevelString(entry.logLevel) << "," << entry.message
                << "," << std::to_string(entry.intervalCount) << ","
                << std::to_string(entry.totalCount) << std::endl;
}

void Logger::writeLineToPlainTextFileHandler(LogEntry entry)
{
  std::string timestamp = "[" + ClockSync::systemTimeToString(entry.lastLogged.systemTime) + "]";

  std::string loggerName = "[" + this->name + "] ";
  std::string levelString = "[" + this->getLevelString(entry.logLevel) + "]: ";

  this->logFile << timestamp << levelString << entry.message << std::endl;
}

void Logger::writeLineToJsonFileHandler(LogEntry entry)
{
  std::string timestamp = ClockSync::systemTimeToString(entry.lastLogged.systemTime);

  // TODO: Convert this to a json format
  this->logFile << timestamp << "," << this->getLevelString(entry.logLevel) << entry.message << ","
                << std::to_string(entry.intervalCount) << "," << std::to_string(entry.totalCount)
                << std::endl;
}
}  // namespace Logging
}  // namespace Firefly
