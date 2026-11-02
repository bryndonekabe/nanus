#pragma once

#include <bedrock/net.hpp>
#include <gfx/mesh.hpp>

// The .nmesh file extension, meant to easily, and efficiently encode mesh data
// for rendering

// All data is stored big-endian.
// Vertex floats are IEEE-754 binary32.
// Vertex and index data immediately follow this header.
// this means you will have to do ntoh in order to get
// reliable data on all machines

// vertex and index data is assumed to be right after this header
namespace nanus::filetypes {
constexpr u8 NMESH_MAGIC[4] = {'N', 'M', 'S', 'H'};

#pragma pack(push, 1)
struct nvert {
  // render data
  nvec3 position;
  nvec3 normal;
  nvec2 uv;

  // skinning data
  u8 bone_ids[4];
  u8 bone_weights[4];
};
struct nface {
  nu32 indices[3];
};
struct nmesh {
  u8 magic[4];
  nu32 num_vertices;
  nu32 num_indices; // NOTE: means number of pairs of 3 / faces
};
#pragma pack(pop)
// after header:
// vertices is x number of nvert
// indices after is x number of nface

// check magic + file size math
bool validate(const nmesh &mesh, usize size);

// convert nmesh -> loaded mesh
// NOTE: this modifies the data of 'mesh' (endianness)
// TODO: this code doesnt account for endianness yet
// NOTE: i *tried* to account for endianness (test it)
inline gfx::Mesh get(nmesh &mesh) {
  byte *raw = (byte *)&mesh;
  // jump to the end of the header
  // which is the start of data
  byte *after = raw + sizeof(mesh);
  u32 n_verts = (u32)mesh.num_vertices;
  u32 n_faces = (u32)mesh.num_indices;
  nvert *v_ptr = (nvert *)after;
  nface *f_ptr = (nface *)(v_ptr + (u32)mesh.num_vertices);

  // as vertices and faces
  gfx::Vertex *vertices = (gfx::Vertex *)v_ptr;
  gfx::Face *faces = (gfx::Face *)f_ptr;

  // change endianness
  // vertices
  for (int i = 0; i < n_verts; ++i) {
    nvert &v = v_ptr[i];
    vertices[i] = gfx::Vertex{
        .position = (vec3)v.position,
        .normal = (vec3)v.normal,
        .uv = (vec2)v.uv,
        .bone_ids = {v.bone_ids[0], v.bone_ids[1], v.bone_ids[2],
                     v.bone_ids[3]},
        .bone_weights = {v.bone_weights[0], v.bone_weights[1],
                         v.bone_weights[2], v.bone_weights[3]},
    };
  }
  // faces
  for (int i = 0; i < n_faces; ++i) {
    auto &f = f_ptr[i];
    faces[i] = gfx::Face{.indices = {
                             (u32)f.indices[0],
                             (u32)f.indices[1],
                             (u32)f.indices[2],
                         }};
  }

  return gfx::Mesh{
      .vertices = View<gfx::Vertex>{vertices, n_verts},
      .faces = View<gfx::Face>{faces, n_faces},
  };
}
} // namespace nanus::filetypes
