#include <platform/io.hpp>

#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>

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

// NOTE: really stupid impl but works technically
// HACK: stupidly just has a pointer casted to a usize and BAM file support
FileHandle fopen(const char *path) {
  FILE *file = ::fopen(path, "r");
  return FileHandle{(usize)file};
}
bool fclose(FileHandle f) { return ::fclose((FILE *)f.id) == 0; }
usize fsize(FileHandle f) {
  FILE *fp = (FILE *)f.id;

  if (fseek(fp, 0, SEEK_END) != 0) {
    return 0; // Error seeking
  }
  usize size = ftell(fp);
  // restore og pos
  fseek(fp, 0, SEEK_SET);
  return size;
}
// returns number of bytes actually read
usize fread(byte *p, usize n, FileHandle f) {
  return ::fread(p, 1, n, (FILE *)f.id);
}
} // namespace nanus::platform
