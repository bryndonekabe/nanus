#pragma once

#include <bedrock/net.hpp>
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
  // offsets into data / path
  nu32 nmesh_offset;
  nu32 ntex_offset;
};
#pragma pack(pop)

// NOTE that the flags are primarily used to store info about how data pointed
// to at *_offset should be interpreted
// currently this means:
constexpr u16 NMDL_FLAG_MESH_REF = 1 << 0;
constexpr u16 NMDL_FLAG_TEX_REF = 1 << 1;

// when a flag is set for either offset, that data can be interpreted as a
// null-terminated ascii string that references a file. This string is a
// relative path based on the file location of the nmdl file itself.
// It is entirely optional, and as a default this option is off.

// interpret pointer as an nmdl
const nmdl *read(const u8 *ptr);

// validate the nmdl
bool validate(const nmdl &mdl);
}; // namespace nanus::filetypes
