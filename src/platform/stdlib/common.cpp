#include <platform/common.hpp>
#include <stdlib.h>

namespace nanus::platform {
bool init() { return true; }
bool deinit() { return true; }
void abort() { ::abort(); };
void exit(int exit_code) { ::exit(exit_code); };
} // namespace nanus::platform
