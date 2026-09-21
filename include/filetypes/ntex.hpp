#pragma once
#include <bedrock/net.hpp>
#include <gfx/texture.hpp>

// This file specifies the .ntex file extension, which is an intentionally
// minimal texture file format, meant to optimize loading of texture data

namespace nanus::filetypes {
constexpr u8 NTEX_MAGIC_ARR[4] = {'N', 'T', 'E', 'X'};

#pragma pack(push, 1)
// NOTE: this is all that is needed, since size is w * h
// and the texture data is assumed rgba8888 and starts directly after the header
// if file size doesn't make sense then you have two options:
// too big, discard ending bytes
// too small, fail to load .ntex
struct ntex {
  u8 magic[4];
  nu16 width;
  nu16 height;
};
#pragma pack(pop)

// interpret pointer as an ntex
const ntex *read(const u8 *ptr);

// validate an ntex based on magic and file size
// NOTE that tex must point to a file buffer already
bool validate(const ntex &tex, usize size);

// after validation, retrieve runtime texture
gfx::Texture get(const ntex &tex, const u8 *ptr);
} // namespace nanus::filetypes
