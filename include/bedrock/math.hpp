#pragma once

#include "../platform/common.hpp"
#include "types.hpp"

namespace nanus {
constexpr f64 e = 2.71828182845904523536;
constexpr f64 log2_e = 1.44269504088896340736;
constexpr f64 log10_e = 0.43429448190325182765;
constexpr f64 ln_2 = 0.69314718055994530942;
constexpr f64 ln_10 = 2.30258509299404568402;

constexpr f64 pi = 3.14159265358979323846;
constexpr f64 pi_2 = 1.57079632679489661923;
constexpr f64 pi_4 = 0.78539816339744830962;
constexpr f64 inv_pi = 0.31830988618379067154;
constexpr f64 two_pi = 0.63661977236758134308;

constexpr f64 two_sqrt_pi = 1.12837916709551257390;
constexpr f64 sqrt_2 = 1.41421356237309504880;
constexpr f64 inv_sqrt_2 = 0.70710678118654752440;

constexpr f64 rad(f64 deg) { return deg * (pi / 180); }
constexpr f64 deg(f64 rad) { return rad * (180 / pi); }
template <typename T> constexpr auto square(T x) { return x * x; }
f64 sqrt(f64 x);
f64 cos(f64 rad);
f64 acos(f64 rad);
f64 sin(f64 rad);
f64 asin(f64 rad);
f64 tan(f64 rad);
f64 atan(f64 rad);

} // namespace nanus
