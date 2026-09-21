#pragma once
#include <bedrock/containers.hpp>
#include <bedrock/mat.hpp>
#include <bedrock/quat.hpp>
#include <bedrock/vec.hpp>

namespace nanus::gfx {

struct PositionKey {
  float time;
  vec3 position;
};
struct RotationKey {
  float time;
  quat rotation;
};
struct ScaleKey {
  float time;
  vec3 scale;
};

struct BoneAnimation {
  u8 id;
  View<PositionKey> positions;
  View<RotationKey> rotations;
  View<ScaleKey> scales;
};
// describe an animation
// NOTE: all time values are in seconds
struct Animation {
  double duration;
  View<BoneAnimation> channels;
};

// interpolate
inline mat4 local_transform(const BoneAnimation &channel, float time) {
  vec3 position{0, 0, 0};
  quat rotation{0, 0, 0, 1};
  vec3 scale{1, 1, 1};

  // NOTE: the if statements are for single-key cases
  if (channel.positions.len == 1) {
    position = channel.positions.ptr[0].position;
  } else {
    for (usize i = 0; i < channel.positions.len - 1; ++i) {
      PositionKey &a = channel.positions.ptr[i];
      PositionKey &b = channel.positions.ptr[i + 1];
      // if t is between a and b
      if (time >= a.time && time <= b.time) {
        // (distance traveled / total distance)
        float u = (time - a.time) / (b.time - a.time);
        position = vec3::lerp(a.position, b.position, u);
      }
    }
  }

  if (channel.rotations.len == 1) {
    rotation = channel.rotations.ptr[0].rotation;
  } else {
    for (usize i = 0; i < channel.rotations.len - 1; ++i) {
      RotationKey &a = channel.rotations.ptr[i];
      RotationKey &b = channel.rotations.ptr[i + 1];
      if (time >= a.time && time <= b.time) {
        float u = (time - a.time) / (b.time - a.time);
        rotation = quat::slerp(a.rotation, b.rotation, u);
      }
    }
  }

  if (channel.scales.len == 1) {
    scale = channel.scales.ptr[0].scale;
  } else {
    for (usize i = 0; i < channel.scales.len - 1; ++i) {
      ScaleKey &a = channel.scales.ptr[i];
      ScaleKey &b = channel.scales.ptr[i + 1];
      if (time >= a.time && time <= b.time) {
        float u = (time - a.time) / (b.time - a.time);
        scale = vec3::lerp(a.scale, b.scale, u);
      }
    }
  }

  return mat4::translate(position) * mat4::rotate(rotation) *
         mat4::scale(scale);
}
// NOTE: we use channels here in favor of a raw array because
// youll end up saving space, since you don't have to encode
// the bones/channels that don't move during the animation
} // namespace nanus::gfx
