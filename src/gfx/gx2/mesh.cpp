#include "util.hpp"
#include <gx2r/draw.h>

// implementation
namespace nanus::gfx {
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
  GX2RDrawIndexed(GX2_PRIMITIVE_MODE_TRIANGLES, &vao.ibuf.buf,
                  GX2_INDEX_TYPE_U32, hdl.face_count * 3,
                  hdl.face_offset * sizeof(Face), hdl.vertex_offset, 1);
}
} // namespace nanus::gfx
