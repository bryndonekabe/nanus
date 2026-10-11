#pragma once

#include <bedrock/types.hpp>

// utility functions specific to filetypes

namespace nanus::filetypes {
inline bool validate_magic(const u8 *header_magic, const u8 *magic) {
  for (int i = 0; i < 4; ++i) {
    if (header_magic[i] != magic[i])
      return false;
  }
  return true;
}
} // namespace nanus::filetypes
