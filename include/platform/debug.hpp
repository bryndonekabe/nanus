#pragma once

// NOTE: All parts of low-level layer or *above* can use this logging system.
// That includes nanus::platform itself

#include <platform/common.hpp>
#include <platform/log.hpp>

#define INFO(fmt, ...) LOG(LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define WARN(fmt, ...) LOG(LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)
#define ERROR(fmt, ...) LOG(LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) LOG(LOG_LEVEL_FATAL, fmt, ##__VA_ARGS__)

#ifdef NANUS_DEBUG
#define DEBUG_PRINT(fmt, ...) LOG(LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) ((void)0)
#endif

// fatal and assertions both panic
#define PANIC()                                                                \
  {                                                                            \
    nanus::platform::abort();                                                  \
  }

#define FATAL(fmt, ...)                                                        \
  do {                                                                         \
    LOG_FATAL(fmt, ##__VA_ARGS__);                                             \
    PANIC();                                                                   \
  } while (0)

// TODO: do your own assertions
#define ASSERT(expr)                                                           \
  do {                                                                         \
    if (!(expr))                                                               \
      FATAL("Assertion " #expr " failed.");                                    \
  } while (0)

// static_assert is cpp language component , not reliant on stdlib
#define STATIC_ASSERT(exp) static_assert(exp && #exp)
