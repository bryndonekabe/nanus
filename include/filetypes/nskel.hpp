#pragma once

#include <bedrock/net.hpp>
#include <gfx/skel.hpp>

// An nskel file holds the skeleton data for a mesh.
// This data is separate so it can potentially be reused
// by multiple different nmeshes
namespace nanus::filetypes {
constexpr u8 NSKEL_MAGIC[4] = {'N', 'S', 'K', 'L'};

#pragma pack(push, 1)
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
gfx::Skeleton get(nskel &skel);
} // namespace nanus::filetypes
