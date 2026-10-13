#pragma once

#include <bedrock/types.hpp>

namespace nanus::platform {
void printf(const char *fmt, ...);
void snprintf(char *str, usize size, const char *fmt, ...);

struct FileHandle {
  usize id;
};
FileHandle fopen(const char *path);
bool fclose(FileHandle f);
usize fsize(FileHandle f);
// returns number of bytes actually read
usize fread(byte *p, usize n, FileHandle f);
// NOTE: there is PURPOSEFULLY no fwrite
// because lots of systems are disk-based and cant just write data onto disk
// NOTE: there is a separate system for writing data that needs to persists
// (savedata)
} // namespace nanus::platform
