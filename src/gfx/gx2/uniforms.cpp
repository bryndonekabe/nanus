#include "util.hpp"
#include <gfx/uniforms.hpp>
#include <gx2/shaders.h>
#include <whb/gfx.h>

void set_block() {
  GX2SetVertexUniformBlock(vs_block->offset, vs_block->size, &uniforms);
  GX2SetPixelUniformBlock(ps_block->offset, ps_block->size, &uniforms);
}

namespace nanus::gfx {
void model(const mat4 &m) {
  uniforms.model = m;
  set_block();
}
void view(const mat4 &m) {
  uniforms.view = m;
  set_block();
}
void proj(const mat4 &m) {
  uniforms.proj = m;
  set_block();
}
void light(const Light &l) {
  uniforms.light_pos = vec4(l.pos, 1.0f);
  uniforms.light_color_intensity = vec4(l.color, l.intensity);
  uniforms.light_color_intensity[3] = l.intensity;
  set_block();
}
void tex(const TextureHandle &t) {
  // helper uniforms for atlas calculation
  vec2 _tex_offset{
      // t.x / float(atlas.width),
      // t.y / float(atlas.height),
  };
  vec2 _tex_scale{
      // t.w / float(atlas.width),
      // t.h / float(atlas.height),
  };
  uniforms.tex_offset_scale[0] = _tex_offset[0];
  uniforms.tex_offset_scale[1] = _tex_offset[1];

  uniforms.tex_offset_scale[2] = _tex_scale[2];
  uniforms.tex_offset_scale[3] = _tex_scale[3];

  set_block();
  // TODO: handle textures
}
} // namespace nanus::gfx
