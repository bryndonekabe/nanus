#pragma once
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <gfx/mesh.hpp>
#include <gfx/texture.hpp>
#include <vector>

using namespace nanus;
using namespace gfx;

// wrapper
struct VertexBuffer {
  static constexpr GLenum buf_type = GL_ARRAY_BUFFER;
  GLuint id;
  // in vertices
  u64 next = 0;
  u64 capacity;

  VertexBuffer(u64 c) : capacity(c) {}
  void init() {
    glGenBuffers(1, &id);
    bind();
    // TODO: GL_DYNAMIC_DRAW or GL_STATIC_DRAW ?
    glBufferData(buf_type, capacity * sizeof(Vertex), nullptr, GL_STATIC_DRAW);
  }
  void deinit() { glDeleteBuffers(1, &id); }
  void bind() { glBindBuffer(buf_type, id); }
  void unbind() { glBindBuffer(buf_type, 0); }

  void sub_data(u64 offset, const View<Vertex> &view) {
    glBufferSubData(buf_type, offset * sizeof(Vertex),
                    view.len * sizeof(Vertex), view.ptr);
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
  static constexpr GLenum buf_type = GL_ELEMENT_ARRAY_BUFFER;
  GLuint id;
  // in faces
  u64 next = 0;
  u64 capacity;

  IndexBuffer(u64 c) : capacity(c) {}
  void init() {
    glGenBuffers(1, &id);
    bind();
    // TODO: GL_DYNAMIC_DRAW or GL_STATIC_DRAW ?
    glBufferData(buf_type, capacity * sizeof(Face), nullptr, GL_STATIC_DRAW);
  }
  void deinit() { glDeleteBuffers(1, &id); }
  void bind() { glBindBuffer(buf_type, id); }
  void unbind() { glBindBuffer(buf_type, 0); }

  void sub_data(u64 offset, const View<Face> &view) {
    glBufferSubData(buf_type, offset * sizeof(Face), view.len * sizeof(Face),
                    view.ptr);
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
struct VertexArray {
  GLuint id;
  // TODO: only 1 million capacity?
  VertexBuffer vbuf{1'000'000};
  IndexBuffer ibuf{1'000'000};
  void init() {
    glGenVertexArrays(1, &id);
    bind();
    vbuf.init();
    ibuf.init();
  }
  void deinit() {
    glDeleteVertexArrays(1, &id);
    vbuf.deinit();
    ibuf.deinit();
  }
  void bind() {
    glBindVertexArray(id);
    vbuf.bind();
    ibuf.bind();
  }
  void unbind() {
    glBindVertexArray(0);
    vbuf.unbind();
    ibuf.unbind();
  }
};

struct TexBuffer {
  static constexpr GLenum buf_type = GL_TEXTURE_2D;

  GLuint id;

  // in pixels
  const u32 width;
  const u32 height;
  u32 x = 0;
  u32 y = 0;
  u32 shelf_height = 0;

  TexBuffer(u32 w, u32 h) : width(w), height(h) {}
  void init() {
    glGenTextures(1, &id);
    bind();
    glTexImage2D(buf_type, 0, GL_RGBA8, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(buf_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(buf_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(buf_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(buf_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(buf_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(buf_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }
  void deinit() { glDeleteTextures(1, &id); }
  void bind() { glBindTexture(buf_type, id); }
  void unbind() { glBindTexture(buf_type, 0); }

  TextureHandle upload(const Texture &tex) {
    if (x + tex.width > width) {
      // Start a new row
      x = 0;
      y += shelf_height;
      shelf_height = 0;
    }

    // Atlas is full / cannot take a texture of this size
    ASSERT(y + tex.height <= height);

    // place at (x,y) with width/height
    u32 old_x = x;
    u32 old_y = y;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(buf_type, 0, x, y, tex.width, tex.height, GL_RGBA,
                    GL_UNSIGNED_BYTE, tex.pixels.ptr);
    // advance forward
    // if new texture is greater in height, increase shelf height
    x += tex.width;
    shelf_height = shelf_height >= tex.height ? shelf_height : tex.height;

    return TextureHandle{
        .x = old_x,
        .y = old_y,
        .w = tex.width,
        .h = tex.height,
    };
  }
};

// globals
inline VertexArray vao{};
// TODO width and height?
inline TexBuffer atlas{4096, 4096};
inline SDL_Window *window;
inline SDL_GLContext ctx;
