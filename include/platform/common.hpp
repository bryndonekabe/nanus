#pragma once
#include "../bedrock/types.hpp"

namespace nanus::platform {
// unfortunately, we need runtime
// to be platform-agnostic in this case
// (msvc/gcc/clang have diff macros for this)
// and honestly its extremely fast so doesnt matter fr
inline const bool little_endian = [] {
  // read first byte of the integer
  // if its 0x04, we know we're little endian
  const u32 n = 0x01020304;
  return *(const u8 *)&n == 0x04;
}();

bool init();
bool deinit();
void abort();
void exit(int exit_code);
} // namespace nanus::platform
