#include <bedrock/types.hpp>
#include <fstream>
#include <iostream>
#include <platform/debug.hpp>
#include <vector>

#include "util.hpp"

using namespace nanus;

void file_write_bytes(const char *path, const std::vector<byte> &bytes) {
  std::ofstream file(path, std::ios::out | std::ios::binary);
  file.write((char *)bytes.data(), bytes.size());
  file.close();
}

int main(int argc, char **argv) {
  // first arg is the file
  // second is the output file
  char *progname = argv[0];
  if (argc < 3) {
    INFO("Incorrect usage. Call as %s <input> <output>", progname);
    return 1;
  }

  char *in_file = argv[1];
  char *out_file = argv[2];

  LoadedModel model = load_model(in_file);

  DEBUG_PRINT("In Vertices: %i\nIndices: %i\nBones: %i\n"
              "Binds: %i\nLocals: %i\nAnims: %i\n"
              "TexW: %i TexH: %i",
              model.vertices.size(), model.indices.size(),
              model.skeleton.size(), model.inverse_binds.size(),
              model.local_transforms.size(), model.animations.size(),
              model.tex.w, model.tex.h);
  for (int i = 0; i < model.animations.size(); ++i) {
    DEBUG_PRINT("Anim: %s", model.animations[i].name);
  }

  std::vector<byte> nmdl_bytes;
  model_to_nmdl(model, nmdl_bytes);

  // write to file of name out_file
  file_write_bytes(out_file, nmdl_bytes);
  // DEBUG_PRINT("Out:");

  return 0;
}
