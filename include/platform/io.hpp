#pragma once

#include <bedrock/types.hpp>

namespace nanus::platform {
void printf(const char *fmt, ...);
void snprintf(char *str, usize size, const char *fmt, ...);
} // namespace nanus::platform
