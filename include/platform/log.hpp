#pragma once

#include <bedrock/ansi.hpp>
#include <platform/io.hpp>

#define CURR_FUNC __func__
#define CURR_FILE __FILE__
#define CURR_LINE __LINE__

namespace nanus::platform {
enum class LogLevel : u8 { Info = 0, Debug, Warn, Error, Fatal };
inline const char *log_keywords[] = {
    ANSI_STR(BWHITE, "INFO"), ANSI_STR(GREEN, "DEBUG"),
    ANSI_STR(YELLOW, "WARN"), ANSI_STR(MAGENTA, "ERROR"),
    ANSI_STR(RED, "FATAL")};

const char *log_str(LogLevel lvl);

template <typename... Args>
void log(LogLevel lvl, const char *fmt, Args... args) {
  // extract formatted message first
  char msg[1024];
  nanus::platform::snprintf(msg, 1024, fmt, args...);

  // insert formatted msg into log format
  static const char *log_fmt = "[%s] %s";
  nanus::platform::printf(log_fmt, log_str(lvl), msg);
  nanus::platform::printf("\n");
}

// just puts the debug info to the beginning
template <typename... Args>
void log_verbose(LogLevel lvl, int line, const char *func, const char *file,
                 const char *fmt, Args... args) {
  // extract formatted message first
  char msg[1024];
  nanus::platform::snprintf(msg, 1024, fmt, args...);

  // insert formatted message into verbose format
  static const char *verbose_fmt = "[%s:%i:%s()] %s";
  log(lvl, verbose_fmt, file, line, func, msg);
}
} // namespace nanus::platform

// defines
#define LOG_LEVEL_INFO nanus::platform::LogLevel::Info
#define LOG_LEVEL_DEBUG nanus::platform::LogLevel::Debug
#define LOG_LEVEL_WARN nanus::platform::LogLevel::Warn
#define LOG_LEVEL_ERROR nanus::platform::LogLevel::Error
#define LOG_LEVEL_FATAL nanus::platform::LogLevel::Fatal

// in release builds, extra debug info isn't present
#define _LOG(level, fmt, ...) nanus::platform::log(level, fmt, ##__VA_ARGS__)
#define _LOG_VERBOSE(level, fmt, ...)                                          \
  nanus::platform::log_verbose(level, CURR_LINE, CURR_FUNC, CURR_FILE, fmt,    \
                               ##__VA_ARGS__)
#ifdef NANUS_DEBUG
#define LOG(level, fmt, ...) _LOG_VERBOSE(level, fmt, ##__VA_ARGS__)
#else
#define LOG(level, fmt, ...) _LOG(level, fmt, ##__VA_ARGS__)
#endif
