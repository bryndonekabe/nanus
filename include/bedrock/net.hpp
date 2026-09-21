#pragma once

#include "bitops.hpp"
#include "math.hpp"
#include "types.hpp"

namespace nanus {

// host to network
// NOTE: this template only works for *UNSIGNED* types!
template <typename T> T hton(T n) {
  // PLEASE NOTE: this is a runtime variable,
  // *not* a compile-time constant
  // so calling this function before the runtime
  // could cause UB
  if (platform::little_endian)
    return bswap(n);
  else
    return n;
}

// reuse the overloads for signed versions + floats
inline i16 hton(i16 n) { return (i16)hton<u16>((u16)n); }
inline i32 hton(i32 n) { return (i32)hton<u32>((u32)n); }
inline i64 hton(i64 n) { return (i64)hton<u64>((u64)n); }
// maintain bits (cant do a normal cast)
inline f32 hton(f32 n) { return bcast<f32>(hton(bcast<u32>(n))); }
inline f64 hton(f64 n) { return bcast<f64>(hton(bcast<u64>(n))); }
// network to host
// NOTE: ntoh and hton are the same operation lol
template <typename T> T ntoh(T n) { return hton(n); }

// this type is always stored in network byte order
// value in network order: x.value
// value in host order: (T)x
template <typename T> struct Net {
  T value;

  explicit Net(T v) : value(hton(v)) {}
  explicit operator T() const { return ntoh(value); }

  Net &operator=(T v) {
    value = hton(v);
    return *this;
  }
  Net &operator+=(T v) { return *this = static_cast<T>(*this) + v; }
  Net &operator-=(T v) { return *this = static_cast<T>(*this) - v; }
  Net &operator*=(T v) { return *this = static_cast<T>(*this) * v; }
  Net &operator/=(T v) { return *this = static_cast<T>(*this) / v; }
  Net &operator%=(T v) { return *this = static_cast<T>(*this) % v; }
  Net &operator&=(T v) { return *this = static_cast<T>(*this) & v; }
  Net &operator|=(T v) { return *this = static_cast<T>(*this) | v; }
  Net &operator^=(T v) { return *this = static_cast<T>(*this) ^ v; }
  Net &operator<<=(u32 n) { return *this = static_cast<T>(*this) << n; }
  Net &operator>>=(u32 n) { return *this = static_cast<T>(*this) >> n; }
  Net &operator++() { return *this += 1; }
  Net &operator--() { return *this -= 1; }
  T operator++(int) {
    T old = static_cast<T>(*this);
    ++*this;
    return old;
  }
  T operator--(int) {
    T old = static_cast<T>(*this);
    --*this;
    return old;
  }
};

using nu16 = Net<u16>;
using nu32 = Net<u32>;
using nu64 = Net<u64>;
using ni16 = Net<i16>;
using ni32 = Net<i32>;
using ni64 = Net<i64>;

using nf32 = Net<f32>;
using nf64 = Net<f64>;

} // namespace nanus
