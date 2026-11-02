#pragma once

#include <bedrock/net.hpp>
#include <gfx/skel.hpp>

// An nskel file holds the skeleton data for a mesh.
// This data is separate so it can potentially be reused
// by multiple different nmeshes
namespace nanus::filetypes {
constexpr u8 NSKEL_MAGIC[4] = {'N', 'S', 'K', 'L'};

#pragma pack(push, 1)
struct nbone {
  u8 parent;
  u8 name[3]; // ascii chars
};
struct nskel {
  u8 magic[4];
  nu32 num_bones;
};
#pragma pack(pop)
/* The following skeleton data is assumed after this header:
- array of bones (Bone)
- array of inverse binds (nmat4)
- array of bind-local transforms (nmat4)
 ** NOTE: ALL DATA IS NETWORK ORDER / BIG ENDIAN **
The number of bones should be the same as the number of inverse binds and
bind-local transforms, which is assumed in calculations */

// check magic + file size math
bool validate(const nskel &skel, usize size);

// convert nskel -> loaded skel
// NOTE: this modifies the data of 'skel' (endianness)
// NOTE: i *tried* to account for endianness (test it)
inline gfx::Skeleton get(nskel &skel) {
  byte *raw = (byte *)&skel;
  // jump to the end of the header
  // which is the start of data
  byte *after = raw + sizeof(skel);

  u32 n = (u32)skel.num_bones;
  gfx::Bone *b_ptr = (gfx::Bone *)after;
  nmat4 *ib_ptr = (nmat4 *)(b_ptr + n); // jump n bones
  nmat4 *bl_ptr = ib_ptr + n;           // jump n ibinds

  // as mat4 now
  mat4 *ibinds = (mat4 *)ib_ptr;
  mat4 *bind_locals = (mat4 *)bl_ptr;

  // now we have to fix endianness of them
  // NOTE: bone is just 4 u8s, no need to endian swap it
  for (int i = 0; i < n; ++i) {
    ibinds[i] = (mat4)ib_ptr[i];
    bind_locals[i] = (mat4)bl_ptr[i];
  }

  return gfx::Skeleton{
      .bones = {b_ptr, (u32)skel.num_bones},
      .inverse_binds = {ibinds, n},
      .bind_locals = {bind_locals, n},
  };
}
} // namespace nanus::filetypes
