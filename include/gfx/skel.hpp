#pragma once

#include <bedrock/containers.hpp>
#include <gfx/mesh.hpp>

namespace nanus::gfx {
struct Skeleton {
  View<Bone> bones;
  View<mat4> inverse_binds;
  View<mat4> bind_locals;
};
} // namespace nanus::gfx
