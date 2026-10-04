#pragma once

#include <gfx/anim.hpp>
#include <gfx/mesh.hpp>
#include <gfx/skel.hpp>
#include <gfx/texture.hpp>

namespace nanus::gfx {
struct Model {
  Mesh mesh;
  Texture tex;
  Skeleton skel;
  View<Animation> anims;
};
} // namespace nanus::gfx
