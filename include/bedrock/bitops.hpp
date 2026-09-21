#pragma once

#include "types.hpp"
namespace nanus {

// useful bitwise / bytewise operations
// NOTE: this template only works for *UNSIGNED* types!
template <typename T> constexpr T bswap(T value) {
  // static_assert(sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8);
  T result = 0;
  for (u32 i = 0; i < sizeof(T); ++i)
    result |= ((value >> (i * 8)) & T(0xff)) << ((sizeof(T) - 1 - i) * 8);
  return result;
}
// intended to show *explicit* bit reinterpretation
template <typename To, typename From> To bcast(const From &value) {
  return *(const To *)&value;
}

}; // namespace nanus
