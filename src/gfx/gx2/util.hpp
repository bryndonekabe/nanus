#pragma once
#include "shader_gsh.hpp"
#include <bedrock/mat.hpp>
#include <bedrock/types.hpp>
#include <bedrock/vec.hpp>
#include <gfx/mesh.hpp>
#include <gfx/texture.hpp>
#include <gx2r/buffer.h>
#include <gx2r/displaylist.h>
#include <gx2r/draw.h>
#include <gx2r/mem.h>
#include <gx2r/resource.h>
#include <gx2r/surface.h>
#include <stddef.h>
#include <string.h>
#include <whb/gfx.h>

using namespace nanus;
using namespace gfx;

struct Uniforms {
  mat4 model;
  mat4 view;
  mat4 proj;
  vec4 light_pos;
  vec4 light_color_intensity;
  vec4 tex_offset_scale;
}; // wrapper
struct VertexBuffer {
  static constexpr GX2RResourceFlags buf_type =
      GX2R_RESOURCE_BIND_VERTEX_BUFFER;
  GX2RBuffer buf;
  // in vertices
  u64 next = 0;
  u64 capacity;

  VertexBuffer(u64 c) : capacity(c) {}
  void init() {
    buf.flags = buf_type | GX2R_RESOURCE_USAGE_CPU_READ |
                GX2R_RESOURCE_USAGE_CPU_WRITE | GX2R_RESOURCE_USAGE_GPU_READ;
    buf.elemSize = sizeof(Vertex);
    buf.elemCount = capacity;
    GX2RCreateBuffer(&buf);

    bind();
  }
  void deinit() { GX2RDestroyBufferEx(&buf, (GX2RResourceFlags)0); }
  void bind() {
    GX2RSetAttributeBuffer(&buf, 0, (u32)sizeof(Vertex), 0);
    GX2RSetAttributeBuffer(&buf, 1, (u32)sizeof(Vertex), 0);
    GX2RSetAttributeBuffer(&buf, 2, (u32)sizeof(Vertex), 0);
    GX2RSetAttributeBuffer(&buf, 3, (u32)sizeof(Vertex), 0);
    GX2RSetAttributeBuffer(&buf, 4, (u32)sizeof(Vertex), 0);
  }
  void unbind() {
    // intentionally empty
  }
  void sub_data(u64 offset, const View<Vertex> &view) {
    Vertex *const buf_ptr =
        (Vertex *const)GX2RLockBufferEx(&buf, GX2R_RESOURCE_USAGE_CPU_WRITE);
    Vertex *const start = buf_ptr + offset;

    // memcpy in terms of bytes
    memcpy(start, view.ptr, view.len * sizeof(Vertex));

    GX2RUnlockBufferEx(&buf, GX2R_RESOURCE_USAGE_CPU_WRITE);
  }
  // returns offset of uploaded vertices (in terms of vertices)
  u64 upload(const View<Vertex> &v) {
    u64 offset = next;

    ASSERT(offset + v.len <= capacity);

    sub_data(offset, v);
    next += v.len;

    return offset;
  }
};
struct IndexBuffer {
  static constexpr GX2RResourceFlags buf_type = GX2R_RESOURCE_BIND_INDEX_BUFFER;
  GX2RBuffer buf;
  // in faces
  u64 next = 0;
  u64 capacity;

  IndexBuffer(u64 c) : capacity(c) {}
  void init() {
    buf.flags = buf_type | GX2R_RESOURCE_USAGE_CPU_READ |
                GX2R_RESOURCE_USAGE_CPU_WRITE | GX2R_RESOURCE_USAGE_GPU_READ;
    buf.elemSize = sizeof(Face);
    buf.elemCount = capacity;
    GX2RCreateBuffer(&buf);

    bind();
  }
  void deinit() { GX2RDestroyBufferEx(&buf, (GX2RResourceFlags)0); }
  void bind() {
    // intentionally empty
  }
  void unbind() {
    // intentionally empty
  }

  void sub_data(u64 offset, const View<Face> &view) {
    Face *const buf_ptr =
        (Face *const)GX2RLockBufferEx(&buf, GX2R_RESOURCE_USAGE_CPU_WRITE);
    Face *const start = buf_ptr + offset;

    // memcpy in terms of bytes
    memcpy(start, view.ptr, view.len * sizeof(Face));

    GX2RUnlockBufferEx(&buf, GX2R_RESOURCE_USAGE_CPU_WRITE);
  }
  // returns offset of uploaded faces (in terms of faces)
  u64 upload(const View<Face> &v) {
    u64 offset = next;

    ASSERT(offset + v.len <= capacity);

    sub_data(offset, v);
    next += v.len;

    return offset;
  }
};
struct UniformBuffer {
  static constexpr GX2RResourceFlags buf_type =
      GX2R_RESOURCE_BIND_UNIFORM_BLOCK;
  GX2RBuffer buf;

  UniformBuffer() {}
  void init() {
    buf.flags = buf_type | GX2R_RESOURCE_USAGE_CPU_READ |
                GX2R_RESOURCE_USAGE_CPU_WRITE | GX2R_RESOURCE_USAGE_GPU_READ;
    buf.elemSize = sizeof(Uniforms);
    buf.elemCount = 1;
    GX2RCreateBuffer(&buf);
  }
  void deinit() { GX2RDestroyBufferEx(&buf, (GX2RResourceFlags)0); }

  Uniforms *lock() {
    return (Uniforms *)GX2RLockBufferEx(&buf, GX2R_RESOURCE_USAGE_CPU_WRITE);
  }
  void unlock() { GX2RUnlockBufferEx(&buf, GX2R_RESOURCE_USAGE_CPU_WRITE); }
};

struct VertexArray {
  WHBGfxShaderGroup group;
  // TODO: only 1 million capacity?
  VertexBuffer vbuf{1'000'000};
  IndexBuffer ibuf{1'000'000};
  UniformBuffer ubuf;
  void init() {
    if (!WHBGfxLoadGFDShaderGroup(&group, 0, shader_gsh)) {
      ERROR("Unable to load GFD");
    }
    WHBGfxInitShaderAttribute(&group, "position", 0, offsetof(Vertex, position),
                              GX2_ATTRIB_FORMAT_FLOAT_32_32_32);
    WHBGfxInitShaderAttribute(&group, "normal", 1, offsetof(Vertex, normal),
                              GX2_ATTRIB_FORMAT_FLOAT_32_32_32);
    WHBGfxInitShaderAttribute(&group, "uv", 2, offsetof(Vertex, uv),
                              GX2_ATTRIB_FORMAT_FLOAT_32_32);
    WHBGfxInitShaderAttribute(&group, "bone_ids", 3, offsetof(Vertex, bone_ids),
                              GX2_ATTRIB_FLAG_INTEGER |
                                  GX2_ATTRIB_TYPE_16_16_16_16);
    WHBGfxInitShaderAttribute(&group, "bone_weights", 4,
                              offsetof(Vertex, bone_weights),
                              GX2_ATTRIB_FORMAT_FLOAT_32_32_32_32);
    WHBGfxInitFetchShader(&group);
    vbuf.init();
    ibuf.init();
    ubuf.init();

    bind();
  }
  void deinit() {
    WHBGfxFreeShaderGroup(&group);
    vbuf.deinit();
    ibuf.deinit();
    ubuf.deinit();
  }
  void bind() {
    GX2SetFetchShader(&group.fetchShader);
    GX2SetVertexShader(group.vertexShader);
    GX2SetPixelShader(group.pixelShader);
    vbuf.bind();
    ibuf.bind();
  }
  void unbind() {
    vbuf.unbind();
    ibuf.unbind();
  }
};

// struct TexBuffer {
//   static constexpr GLenum buf_type = GL_TEXTURE_2D;

//   GLuint id;

//   // in pixels
//   const u32 width;
//   const u32 height;
//   u32 x = 0;
//   u32 y = 0;
//   u32 shelf_height = 0;

//   TexBuffer(u32 w, u32 h) : width(w), height(h) {}
//   void init() {
//     glGenTextures(1, &id);
//     bind();
//     glTexImage2D(buf_type, 0, GL_RGBA8, width, height, 0, GL_RGBA,
//                  GL_UNSIGNED_BYTE, nullptr);
//     glTexParameteri(buf_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameteri(buf_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     glTexParameteri(buf_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//     glTexParameteri(buf_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//     // glTexParameteri(buf_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     // glTexParameteri(buf_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//   }
//   void deinit() { glDeleteTextures(1, &id); }
//   void bind() { glBindTexture(buf_type, id); }
//   void unbind() { glBindTexture(buf_type, 0); }

//   TextureHandle upload(const Texture &tex) {
//     if (x + tex.width > width) {
//       // Start a new row
//       x = 0;
//       y += shelf_height;
//       shelf_height = 0;
//     }

//     // Atlas is full / cannot take a texture of this size
//     ASSERT(y + tex.height <= height);

//     // place at (x,y) with width/height
//     u32 old_x = x;
//     u32 old_y = y;
//     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//     glTexSubImage2D(buf_type, 0, x, y, tex.width, tex.height, GL_RGBA,
//                     GL_UNSIGNED_BYTE, tex.pixels.ptr);
//     // advance forward
//     // if new texture is greater in height, increase shelf height
//     x += tex.width;
//     shelf_height = shelf_height >= tex.height ? shelf_height : tex.height;

//     return TextureHandle{
//         .x = old_x,
//         .y = old_y,
//         .w = tex.width,
//         .h = tex.height,
//     };
//   }
// };

// globals
inline VertexArray vao{};
inline Uniforms uniforms;
// TODO width and height?
// inline TexBuffer atlas{4096, 4096};

inline vec4 clear_col;

inline GX2UniformBlock *vs_block = nullptr;
inline GX2UniformBlock *ps_block = nullptr;
