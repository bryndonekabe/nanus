#include <coreinit/debug.h>
#include <platform/io.hpp>
#include <stdarg.h>
#include <stdio.h>

namespace nanus::platform {
void printf(const char *fmt, ...) {
  char buffer[1024];

  va_list args;
  va_start(args, fmt);

  ::vsnprintf(buffer, sizeof(buffer), fmt, args);

  va_end(args);

  OSReport("%s", buffer);
}
void snprintf(char *str, usize size, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  ::vsnprintf(str, size, fmt, args);

  va_end(args);
}
} // namespace nanus::platform
