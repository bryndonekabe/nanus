#include "util.hpp"
#include <gfx/mesh.hpp>
#include <stddef.h>

// implementation
namespace nanus::gfx {

mat4 global_transform(u8 bone_id, const View<Bone> &skeleton,
                      const View<mat4> &transforms) {
  Bone &bone = skeleton.ptr[bone_id];
  mat4 &local_transform = transforms.ptr[bone_id];
  u8 parent = bone.parent;

  if (parent == bone_id)
    return local_transform;
  else
    return local_transform * global_transform(parent, skeleton, transforms);
}
MeshHandle submit(const Mesh &m) {
  u64 vert_off = vao.vbuf.upload(m.vertices);
  u64 face_off = vao.ibuf.upload(m.faces);

  return MeshHandle{
      .vertex_offset = vert_off,
      .vertex_count = m.vertices.len,
      .face_offset = face_off,
      .face_count = m.faces.len,
  };
}

void draw(MeshHandle hdl) {
  glDrawElementsBaseVertex(GL_TRIANGLES, hdl.face_count * 3, GL_UNSIGNED_INT,
                           (void *)(hdl.face_offset * sizeof(Face)),
                           hdl.vertex_offset);
}
} // namespace nanus::gfx
