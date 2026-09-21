#include <platform/common.hpp>
#include <stdlib.h>
#include <whb/proc.h>

namespace nanus::platform {
bool init() {
  WHBProcInit();
  return true;
}
bool deinit() {
  WHBProcShutdown();
  return true;
}
void abort() { ::abort(); };
void exit(int exit_code) { ::exit(exit_code); };
} // namespace nanus::platform
