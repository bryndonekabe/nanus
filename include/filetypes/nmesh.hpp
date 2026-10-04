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
struct nmesh {
  u8 magic[4];
  nu32 num_vertices;
  nu32 num_indices;
};
#pragma pack(pop)

// check magic + file size math
bool validate(const nmesh &mesh, usize size);

// convert nmesh -> loaded mesh
// NOTE: this modifies the data of 'mesh' (endianness)
gfx::Mesh get(nmesh &mesh);
} // namespace nanus::filetypes
