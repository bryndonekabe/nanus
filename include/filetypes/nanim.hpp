#pragma once

#include <bedrock/net.hpp>
#include <gfx/anim.hpp>

// A nanim file holds a single animation for a skeleton
// Again, this file is separate to be potentially reused
// by multiple different skeletons
namespace nanus::filetypes {
constexpr u8 NANIM_MAGIC[4] = {'N', 'A', 'N', 'M'};

#pragma pack(push, 1)
struct nposkey {
  nf32 time;
  nvec3 position;
};
struct nrotkey {
  nf32 time;
  nquat rotation;
};
struct nscalekey {
  nf32 time;
  nvec3 scale;
};
struct nboneanim {
  u8 bone_id;
  nu32 num_positions;
  nu32 num_rotations;
  nu32 num_scales;
};
struct nanim {
  u8 magic[4];
  u8 name[32];
  nf32 duration;
  nu32 num_channels;
};
#pragma pack(pop)

/* The following animation data is assumed after this header:
- array of channels (nboneanim)
-- array of positions (nposkey)
-- array of rotations (nrotkey)
-- array of scales (nscalekey)
 ** NOTE: ALL DATA IS NETWORK ORDER / BIG ENDIAN ** */

// check magic + file size math
bool validate(const nanim &anim, usize size);

// convert nanim -> loaded anim
// NOTE: this modifies the data of 'skel' (endianness)
gfx::Animation get(nanim &anim);
} // namespace nanus::filetypes
