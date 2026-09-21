#pragma once
#include <bedrock/containers.hpp>
#include <bedrock/mat.hpp>
#include <bedrock/vec.hpp>

namespace nanus::gfx {

struct Vertex {
  // render data
  vec3 position;
  vec3 normal;
  vec2 uv;

  // skinning data
  u8 bone_ids[4];
  u8 bone_weights[4];
};
struct Face {
  u32 indices[3];
};
struct Bone {
  u8 parent;  // index into array
  u8 name[3]; // 3 byte ASCII ident
};
constexpr usize MAX_BONES = 256;

struct Mesh {
  View<Vertex> vertices;
  View<Face> faces;
  // View<Bone> skeleton;
  // View<mat4> inverse_binds; // binds for skeleton[i]
};

// NOTE: recursively get global transform of a bone
// NOTE: when a bone's parent is itself, it is considered root
// static inline mat4 global_transform(u8 bone_id, const View<Bone> &skeleton,
//                                     const View<mat4> &transforms) {
//   Bone &bone = skeleton.ptr[bone_id];
//   mat4 &local_transform = transforms.ptr[bone_id];
//   u8 parent = bone.parent;

//   if (parent == bone_id)
//     return local_transform;
//   else
//     return local_transform * global_transform(parent, skeleton, transforms);
// }

struct MeshHandle {
  u64 vertex_offset;
  u64 vertex_count;
  u64 face_offset;
  u64 face_count;
};

MeshHandle submit(const Mesh &m);
void draw(MeshHandle hdl);
} // namespace nanus::gfx
