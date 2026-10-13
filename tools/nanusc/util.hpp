#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <gfx/anim.hpp>
#include <gfx/mesh.hpp>
#include <gfx/texture.hpp>

#include <vector>

using namespace nanus;
using namespace gfx;

struct LoadedBoneAnimation {
  u8 id;
  std::vector<PositionKey> positions;
  std::vector<RotationKey> rotations;
  std::vector<ScaleKey> scales;
};
struct LoadedAnimation {
  double duration;
  std::vector<LoadedBoneAnimation> channels;
  const char *name;
};

struct LoadedTexture {
  u32 w, h;
  std::vector<Pixel> pixels;
};

struct LoadedModel {
  std::vector<Vertex> vertices;
  std::vector<Face> indices;
  std::vector<Bone> skeleton;

  std::vector<mat4> inverse_binds;
  std::vector<mat4> local_transforms;

  LoadedTexture tex;

  std::vector<LoadedAnimation> animations;
};

LoadedModel load_model(const char *path);

// usize add_mesh(LoadedModel &model, std::vector<byte> &bytes);
// usize add_tex(LoadedModel &model, std::vector<byte> &bytes);
// usize add_skel(LoadedModel &model, std::vector<byte> &bytes);
// usize add_anim(LoadedModel &model, std::vector<byte> &bytes);

void model_to_nmdl(LoadedModel &model, std::vector<byte> &bytes);
