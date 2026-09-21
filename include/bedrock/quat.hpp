#pragma once
#include "math.hpp"
#include "vec.hpp"

// quaternion implementation, built over top of tvec4
namespace nanus {
template <typename T> struct tquat {
  union {
    tvec4<T> vec;
    T elements[4];
    struct {
      T x, y, z, w;
    };
  };

  // ctors
  // NOTE: default ctor here is the identity quaternion
  constexpr tquat() : x(0), y(0), z(0), w(1) {}
  constexpr tquat(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
  constexpr tquat(const tvec4<T> &v) : vec(v) {}

  // statics
  constexpr static tquat identity() { return tquat{0, 0, 0, 1}; }
  // generate a quaternion from an axis + radians rotation
  static tquat from_axis_angle(const tvec3<T> &axis, T angle) {
    tvec3<T> normalized_axis = axis.normalized();
    T half_angle = angle / T(2);
    T sin_half = nanus::sin(half_angle);
    return tquat{normalized_axis.x * sin_half, normalized_axis.y * sin_half,
                 normalized_axis.z * sin_half, nanus::cos(half_angle)};
  }
  constexpr static T dot(const tquat &a, const tquat &b) {
    // like normal dot product
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
  }
  // spherical linear interpolation between two quats
  static tquat slerp(const tquat &a, const tquat &b, T t) {
    tquat q1 = a;
    tquat q2 = b;
    T d = dot(q1, q2); // angle between a and b
    // q and -q represent the same rotation.
    // Negating one chooses the shortest interpolation path.
    if (d < T(0)) {
      q2 = -q2;
      d = -d;
    }
    // when angle is very close: lerp instead of dividing by sin(theta).
    if (d > T(0.9995)) {
      return (q1 + (q2 - q1) * t).normalized();
    }

    T theta = nanus::acos(d);
    T s = nanus::sin(theta);
    T w1 = nanus::sin((T(1) - t) * theta) / s;
    T w2 = nanus::sin(t * theta) / s;

    return q1 * w1 + q2 * w2;
  }

  // methods
  constexpr auto length_squared() const { return vec.length_squared(); }
  auto length() const { return vec.length(); }
  tquat normalized() const { return vec.normalized(); }
  constexpr tquat conjugate() const { return tquat{-x, -y, -z, w}; }
  constexpr tquat inverse() const { return conjugate() / length_squared(); }
  // NOTE: this is hamilton product,
  // *NOT* a simple component-wise multiplication
  constexpr tquat operator*(const tquat &q) const {
    return tquat{w * q.x + x * q.w + y * q.z - z * q.y,
                 w * q.y - x * q.z + y * q.w + z * q.x,
                 w * q.z + x * q.y - y * q.x + z * q.w,
                 w * q.w - x * q.x - y * q.y - z * q.z};
  }
  // NOTE: rotate vector v by 'this' rotation quaternion
  // NOTE: assumes a normalized quaternion
  tvec3<T> rotate(const tvec3<T> &v) const {
    tvec3<T> u{x, y, z};
    tvec3<T> uv = tvec3<T>::cross(u, v);
    tvec3<T> uuv = tvec3<T>::cross(u, uv);
    return v + T(2) * (w * uv + uuv);
  }

  // operators
  constexpr tquat operator-() const { return tquat{-x, -y, -z, -w}; }
  constexpr tquat operator+(const tquat &q) const {
    return tquat{x + q.x, y + q.y, z + q.z, w + q.w};
  }
  constexpr tquat operator-(const tquat &q) const {
    return tquat{x - q.x, y - q.y, z - q.z, w - q.w};
  }
  // NOTE: this is normal component-wise multiplication by a scalar
  constexpr tquat operator*(T s) const {
    return tquat{x * s, y * s, z * s, w * s};
  }
  constexpr friend tquat operator*(T s, const tquat &q) { return q * s; }
  constexpr tquat operator/(T s) const {
    return tquat{x / s, y / s, z / s, w / s};
  }
};

using quat = tquat<f32>;
using dquat = tquat<f64>;
using nquat = tquat<nf32>;
using ndquat = tquat<nf64>;
} // namespace nanus
