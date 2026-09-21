#include <platform/io.hpp>

#include <stdarg.h>
#include <stdio.h>

namespace nanus::platform {
void printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  int n = ::vprintf(fmt, args);

  va_end(args);
}

void snprintf(char *str, usize size, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  ::vsnprintf(str, size, fmt, args);

  va_end(args);
}
} // namespace nanus::platform
