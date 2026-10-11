#include <bedrock/types.hpp>
#include <iostream>
#include <platform/debug.hpp>
#include <vector>

using namespace nanus;

template <typename T>
void add_bytes(const T &element, std::vector<byte> &bytes) {
  const byte *raw = (const byte *)&element;
  for (int i = 0; i < sizeof(T); ++i) {
    bytes.push_back(raw[i]);
  }
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
  return 0;
}
