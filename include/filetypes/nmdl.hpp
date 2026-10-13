#pragma once

#include <bedrock/net.hpp>
#include <gfx/anim.hpp>
#include <gfx/mesh.hpp>
#include <gfx/model.hpp>

#include <filetypes/nanim.hpp>
#include <filetypes/nmesh.hpp>
#include <filetypes/nskel.hpp>
#include <filetypes/ntex.hpp>
// An nmdl file is the top-level file that holds model data, which is the
// combination of mesh data and texture data It is meant primarily for speed,
// data efficiency, and ease of use This means there are little to no
// configurable flags or parameters Data is as-is and can be loaded onto the GPU
// nearly instantly from disk NOTE: all data is in *network-order*, but these
// types allow for implicit endianness conversion

namespace nanus::filetypes {
constexpr u8 NMDL_MAGIC[4] = {'N', 'M', 'D', 'L'};
constexpr u16 NMDL_VERSION = 1;

#pragma pack(push, 1)
struct nmdl {
  u8 magic[4];
  nu16 version; // version must == current to work on nanus
  nu16 flags;   // see below

  // offsets into data or path
  nu32 nmesh_offset;
  nu32 ntex_offset;
  nu32 nskel_offset;

  nu32 num_anims;
  nu32 nanim_offset;
};
#pragma pack(pop)

// NOTE that the flags are primarily used to store info about how data pointed
// to at *_offset should be interpreted
// currently this means:
constexpr u16 NMDL_FLAG_MESH_REF = 1 << 0;
constexpr u16 NMDL_FLAG_TEX_REF = 1 << 1;

// TODO: implement this stuff here:
// TODO: how to add flags for animations?
/*
when a flag is set for either offset, that data can be interpreted as a
null-terminated ascii string that references a file. This string is a relative
path based on the file location of the nmdl file itself. It is entirely
optional, and as a default this option is off.
 */

// validate the nmdl
bool validate(const nmdl &mdl);
// go from nmdl -> loaded model
// NOTE: this modifies data
inline gfx::Model get(nmdl &mdl) {
  byte *raw = (byte *)&mdl;

  byte *mesh = raw + (u32)mdl.nmesh_offset;
  byte *tex = raw + (u32)mdl.ntex_offset;
  byte *skel = raw + (u32)mdl.nskel_offset;

  // TODO: have to treat animations a bit differently
  // NOTE: not sure what to do here?
  byte *raw_anims = raw + (u32)mdl.nanim_offset;
  View<gfx::Animation> anims{nullptr, 0};

  gfx::Model out{.mesh = get(*(nmesh *)mesh),
                 .tex = get(*(ntex *)tex),
                 .skel = get(*(nskel *)skel),
                 .anims = anims};
  return out;
}
}; // namespace nanus::filetypes
