#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <gfx/anim.hpp>
#include <gfx/mesh.hpp>
#include <gfx/texture.hpp>

#include <vector>

using namespace nanus;
using namespace gfx;

// recursively get the global transform of a bone
inline mat4 bone_global(usize bone_id, const std::vector<Bone> &skeleton,
                        const std::vector<mat4> &local_transforms) {
  u8 parent = skeleton[bone_id].parent;
  if (parent == bone_id)
    return local_transforms[bone_id];
  else
    return bone_global(parent, skeleton, local_transforms) *
           local_transforms[bone_id];
}
// calculate bone matrix global transforms
inline std::vector<mat4>
bone_globals(const std::vector<Bone> &skeleton,
             const std::vector<mat4> &local_transforms) {
  std::vector<mat4> result;
  for (usize i = 0; i < skeleton.size(); ++i) {
    result.push_back(bone_global(i, skeleton, local_transforms));
  }

  return result;
}
// apply skinning
inline std::vector<mat4> bone_final(const std::vector<mat4> &globals,
                                    const std::vector<mat4> &inverse_binds) {
  std::vector<mat4> result;
  for (usize i = 0; i < globals.size(); ++i) {
    result.push_back(globals[i] * inverse_binds[i]);
  }
  return result;
}

struct LoadedBoneAnimation {
  u8 id;
  std::vector<PositionKey> positions;
  std::vector<RotationKey> rotations;
  std::vector<ScaleKey> scales;
};
struct LoadedAnimation {
  double duration;
  std::vector<LoadedBoneAnimation> channels;
};

struct LoadedTexture {
  u32 w, h;
  std::vector<Pixel> pixels;
};

struct LoadedMesh {
  std::vector<Vertex> vertices;
  std::vector<Face> indices;
  std::vector<Bone> skeleton;

  std::vector<mat4> inverse_binds;
  std::vector<mat4> local_transforms;

  LoadedTexture tex;

  std::vector<LoadedAnimation> animations;
};

inline std::vector<mat4>
anim_locals(LoadedAnimation &anim, std::vector<mat4> &default_locals, float t) {
  // replace non-affected bones w/ default locals,
  // and the rest with their interpolated transform
  std::vector<mat4> result = default_locals;
  for (LoadedBoneAnimation &channel : anim.channels) {
    result[channel.id] = local_transform(
        BoneAnimation{channel.id,
                      {channel.positions.data(), channel.positions.size()},
                      {channel.rotations.data(), channel.rotations.size()},
                      {channel.scales.data(), channel.scales.size()}},
        t);
  }
  return result;
}
LoadedMesh load_mesh(const char *path);
