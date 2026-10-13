#include "util.hpp"
#include <filetypes/nmdl.hpp>

using namespace nanus;
using namespace filetypes;

template <typename T>
void add_bytes(const T &element, std::vector<byte> &bytes) {
  const byte *raw = (const byte *)&element;
  for (int i = 0; i < sizeof(T); ++i) {
    bytes.push_back(raw[i]);
  }
}
void add_ptr_bytes(byte *ptr, usize n, std::vector<byte> &bytes) {
  for (int i = 0; i < n; ++i) {
    bytes.push_back(ptr[i]);
  }
}
void add_nmats(mat4 *ptr, usize n, std::vector<byte> &bytes) {
  for (int i = 0; i < n; ++i) {
    mat4 m = ptr[i];
    nmat4 mat{
        nvec4(nf32(m.col0.x), nf32(m.col0.y), nf32(m.col0.z), nf32(m.col0.w)),
        nvec4(nf32(m.col1.x), nf32(m.col1.y), nf32(m.col1.z), nf32(m.col1.w)),
        nvec4(nf32(m.col2.x), nf32(m.col2.y), nf32(m.col2.z), nf32(m.col2.w)),
        nvec4(nf32(m.col3.x), nf32(m.col3.y), nf32(m.col3.z), nf32(m.col3.w))};
    add_bytes(mat, bytes);
  }
}

// NOTE: n counts as starting offset inside the buffer so just use that
void add_verts(LoadedModel &model, std::vector<byte> &bytes) {
  for (int i = 0; i < model.vertices.size(); i++) {
    nvert vert{
        .position = nvec3(nf32(model.vertices[i].position.x),
                          nf32(model.vertices[i].position.y),
                          nf32(model.vertices[i].position.z)),
        .normal = nvec3(nf32(model.vertices[i].normal.x),
                        nf32(model.vertices[i].normal.y),
                        nf32(model.vertices[i].normal.z)),
        .uv = nvec2(nf32(model.vertices[i].uv.x), nf32(model.vertices[i].uv.y)),
        .bone_ids = {model.vertices[i].bone_ids[0],
                     model.vertices[i].bone_ids[1],
                     model.vertices[i].bone_ids[2],
                     model.vertices[i].bone_ids[3]},
        .bone_weights = {model.vertices[i].bone_weights[0],
                         model.vertices[i].bone_weights[1],
                         model.vertices[i].bone_weights[2],
                         model.vertices[i].bone_weights[3]},
    };
    add_bytes(vert, bytes);
  }
}
void add_faces(LoadedModel &model, std::vector<byte> &bytes) {
  for (int i = 0; i < model.indices.size(); ++i) {
    nface face{.indices = {nu32(model.indices[i].indices[0]),
                           nu32(model.indices[i].indices[1]),
                           nu32(model.indices[i].indices[2])}};
    add_bytes(face, bytes);
  }
}
usize add_mesh(LoadedModel &model, std::vector<byte> &bytes) {
  nmesh mesh{
      .magic = {NMESH_MAGIC[0], NMESH_MAGIC[1], NMESH_MAGIC[2], NMESH_MAGIC[3]},
      .num_vertices = nu32(model.vertices.size()),
      .num_indices = nu32(model.indices.size())};
  usize n = bytes.size();
  add_bytes(mesh, bytes);
  add_verts(model, bytes);
  add_faces(model, bytes);
  return n;
}

usize add_tex(LoadedModel &model, std::vector<byte> &bytes) {
  // tex is already rgba8888 so we're actually good
  // no need to convert to big endian
  ntex tex{
      .magic = {NTEX_MAGIC[0], NTEX_MAGIC[1], NTEX_MAGIC[2], NTEX_MAGIC[3]},
      .width = nu16(model.tex.w),
      .height = nu16(model.tex.h)};
  usize n = bytes.size();
  add_bytes(tex, bytes);
  add_ptr_bytes((byte *)model.tex.pixels.data(), model.tex.w * model.tex.h,
                bytes);
  return n;
}

usize add_skel(LoadedModel &model, std::vector<byte> &bytes) {
  nskel skel{
      .magic = {NSKEL_MAGIC[0], NSKEL_MAGIC[1], NSKEL_MAGIC[2], NSKEL_MAGIC[3]},
      .num_bones = nu32(model.skeleton.size())};
  usize n = bytes.size();
  add_bytes(skel, bytes);
  // add bones, then inverse binds, then local transforms
  add_ptr_bytes((byte *)model.skeleton.data(),
                model.skeleton.size() * sizeof(Bone), bytes);
  add_nmats(model.inverse_binds.data(), model.inverse_binds.size(), bytes);
  add_nmats(model.local_transforms.data(), model.local_transforms.size(),
            bytes);
  return n;
}
// HACK: actually support anims correctly chud
usize add_anims(LoadedModel &model, std::vector<byte> &bytes) { return 0; }

void model_to_nmdl(LoadedModel &model, std::vector<byte> &bytes) {

  nmdl mdl_header{
      .magic = {NMDL_MAGIC[0], NMDL_MAGIC[1], NMDL_MAGIC[2], NMDL_MAGIC[3]},
      .version = nu16(NMDL_VERSION),
      .flags = nu16(0),
      .nmesh_offset = nu32(0),
      .ntex_offset = nu32(0),
      .nskel_offset = nu32(0),
      // NOTE: fix animations brodie
      .num_anims = nu32(0),
      .nanim_offset = nu32(0)};
  add_bytes(mdl_header, bytes);
  // HACK: now we've gotta get the header backit back to properly set the
  // offsets should just be at start
  nmdl *as_nmdl = (nmdl *)bytes.data();
  u32 mesh_off = add_mesh(model, bytes);
  u32 tex_off = add_tex(model, bytes);
  u32 skel_off = add_skel(model, bytes);
  u32 anim_off = add_anims(model, bytes);
  as_nmdl->nmesh_offset = mesh_off;
  as_nmdl->ntex_offset = tex_off;
  as_nmdl->nskel_offset = skel_off;
  as_nmdl->nanim_offset = anim_off;
}
