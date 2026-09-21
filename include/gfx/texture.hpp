#pragma once
#include <bedrock/containers.hpp>
#include <bedrock/types.hpp>

namespace nanus::gfx {
struct Pixel {
  u8 r;
  u8 g;
  u8 b;
  u8 a;
};
struct Texture {
  View<Pixel> pixels;
  u32 width, height;
};
struct TextureHandle {
  u32 x, y, w, h;
};

TextureHandle submit(const Texture &t);
} // namespace nanus::gfx
