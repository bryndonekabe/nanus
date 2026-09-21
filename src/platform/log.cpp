#include <platform/io.hpp>
#include <platform/log.hpp>

namespace nanus::platform {
const char *log_str(LogLevel lvl) { return log_keywords[static_cast<u8>(lvl)]; }
} // namespace nanus::platform
