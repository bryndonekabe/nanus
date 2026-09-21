#pragma once
#include <bedrock/containers.hpp>
#include <bedrock/mat.hpp>
#include <gfx/texture.hpp>

namespace nanus::gfx {
struct Light {
  vec3 pos;
  vec3 color;
  f32 intensity;
};
void model(const mat4 &m);
void view(const mat4 &m);
void proj(const mat4 &m);
void light(const Light &l);
void tex(const TextureHandle &t);
void bones(const View<mat4> &b);
} // namespace nanus::gfx
