#include "util.hpp"
#include <GL/glew.h>
#include <gfx/uniforms.hpp>
#include <platform/debug.hpp>

#define UNIFORM(name)                                                          \
  GLint curr_prog = 0;                                                         \
  glGetIntegerv(GL_CURRENT_PROGRAM, &curr_prog);                               \
  int loc;                                                                     \
  loc = glGetUniformLocation(curr_prog, #name)
// NOTE: m is an lvalue here
#define UNIFORM_MATRIX(name, m)                                                \
  {                                                                            \
    UNIFORM(name);                                                             \
    glUniformMatrix4fv(loc, 1, GL_FALSE, (GLfloat *)&m);                       \
  }
// NOTE: m is a pointer not a lvalue here
#define UNIFORM_MATV(name, m, count)                                           \
  {                                                                            \
    UNIFORM(name);                                                             \
    glUniformMatrix4fv(loc, count, GL_FALSE, (GLfloat *)m);                    \
  }

#define UNIFORM_VEC3(name, v)                                                  \
  {                                                                            \
    UNIFORM(name);                                                             \
    glUniform3fv(loc, 1, (GLfloat *)&v);                                       \
  }
#define UNIFORM_VEC2(name, v)                                                  \
  {                                                                            \
    UNIFORM(name);                                                             \
    glUniform2fv(loc, 1, (GLfloat *)&v);                                       \
  }

#define UNIFORM_INT(name, i)                                                   \
  {                                                                            \
    UNIFORM(name);                                                             \
    glUniform1i(loc, i);                                                       \
  }
#define UNIFORM_FLOAT(name, f)                                                 \
  {                                                                            \
    UNIFORM(name);                                                             \
    glUniform1f(loc, f);                                                       \
  }

namespace nanus::gfx {
void model(const mat4 &m) { UNIFORM_MATRIX(model, m); }
void view(const mat4 &m) { UNIFORM_MATRIX(view, m); }
void proj(const mat4 &m) { UNIFORM_MATRIX(proj, m); }
void light(const Light &l) {
  UNIFORM_VEC3(light.pos, l.pos);
  UNIFORM_VEC3(light.color, l.color);
  UNIFORM_FLOAT(light.intensity, l.intensity);
}
void tex(const TextureHandle &t) {
  // helper uniforms for atlas calculation
  vec2 _tex_offset{
      t.x / float(atlas.width),
      t.y / float(atlas.height),
  };
  vec2 _tex_scale{
      t.w / float(atlas.width),
      t.h / float(atlas.height),
  };
  UNIFORM_VEC2(_tex_offset, _tex_offset);
  UNIFORM_VEC2(_tex_scale, _tex_scale);

  // atlas is texture number 0
  UNIFORM_INT(atlas, 0);
}
void bones(const View<mat4> &b) { UNIFORM_MATV(bones, b.ptr, b.len); }

#undef UNIFORM_MATRIX
#undef UNIFORM_FLOAT
#undef UNIFORM_INT
#undef UNIFORM_VEC2
#undef UNIFORM_VEC3
#undef UNIFORM
} // namespace nanus::gfx
