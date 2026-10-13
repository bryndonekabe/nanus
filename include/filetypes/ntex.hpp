#pragma once
#include <bedrock/net.hpp>
#include <filetypes/util.hpp>
#include <gfx/texture.hpp>

// This file specifies the .ntex file extension, which is an intentionally
// minimal texture file format, meant to optimize loading of texture data

namespace nanus::filetypes {
constexpr u8 NTEX_MAGIC[4] = {'N', 'T', 'E', 'X'};

#pragma pack(push, 1)
// NOTE: this is all that is needed, since size is w * h
// and the texture data is assumed rgba8888 and starts directly after the header
// if file size doesn't make sense then you have two options:
// too big, discard ending bytes
// too small, fail to load .ntex
// NOTE: width and height are in pixels, not bytes
struct ntex {
  u8 magic[4];
  nu16 width;
  nu16 height;
};
#pragma pack(pop)

// validate an ntex based on magic and file size
// NOTE that tex must point to a file buffer already
inline bool validate(const ntex &tex, usize size) {
  bool correct_magic = validate_magic(tex.magic, NTEX_MAGIC);
  bool correct_size = false;

  // remove header size from calculation
  usize without_header = size - sizeof(ntex);
  // convert from num bytes to num pixels
  usize in_pixels = without_header / sizeof(gfx::Pixel);
  usize tex_size = (u16)tex.width * (u16)tex.height;
  if (in_pixels >= tex_size)
    correct_size = true;

  return correct_magic && correct_size;
}

// after validation, retrieve runtime texture
inline gfx::Texture get(ntex &tex) {
  byte *raw = (byte *)&tex;
  // jump to the end of the header
  // which is the start of texture data
  byte *after = raw + sizeof(ntex);
  gfx::Pixel *tex_ptr = (gfx::Pixel *)after;

  u16 h = (u16)tex.height;
  u16 w = (u16)tex.width;
  usize size = h * w;
  return gfx::Texture{
      .pixels = View<gfx::Pixel>{tex_ptr, size},
      .width = w,
      .height = h,
  };
}

} // namespace nanus::filetypes
