#include "util.hpp"
#include <gfx/texture.hpp>

namespace nanus::gfx {
TextureHandle submit(const Texture &t) { return atlas.upload(t); }
} // namespace nanus::gfx
