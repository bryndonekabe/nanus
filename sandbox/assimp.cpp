#include "util.hpp"
#include <unordered_map>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/* *** UTILITY FUNCTIONS *** */
mat4 convert_matrix(const aiMatrix4x4 &m) {
  // NOTE: assimp matrices are *row-major*
  // while OURS are *column-major*
  // so, we just transpose at the end (gg ez)
  mat4 result;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      result[i][j] = m[i][j];
  return result.transpose();
}
Pixel convert_pix(const aiTexel &texel) {
  return Pixel{.r = texel.r, .g = texel.g, .b = texel.b, .a = texel.a};
}
LoadedTexture convert_tex(const aiTexture *tex) {
  LoadedTexture out;
  usize w = tex->mWidth;
  usize h = tex->mHeight;
  const aiTexel *data = tex->pcData;

  // HACK: this means data is compressed (just leave it for now)
  if (h == 0) {
    // when h is 0, tex size in bytes is mwidth;
    u32 tex_size = w;

    // NOTE: 4 forces it to rgb
    int width, height, channels;
    unsigned char *raw = stbi_load_from_memory(
        (const stbi_uc *)tex->pcData, tex_size, &width, &height, &channels, 4);
    Pixel *pixels = (Pixel *)raw;
    for (usize i = 0; i < width * height; ++i) {
      out.pixels.push_back(pixels[i]);
    }
    stbi_image_free(raw);

    out.w = width;
    out.h = height;
    return out;
  } else {
    // push back pixels into *our* order
    for (usize i = 0; i < w * h; ++i) {
      out.pixels.push_back(convert_pix(data[i]));
    }
    out.w = w;
    out.h = h;
  }

  return out;
}
void dump_nodes(const aiNode *node, int depth = 0) {
  for (int i = 0; i < depth; ++i)
    std::printf("  ");

  std::printf("%s  node=%p parent=%p meshes=%u\n", node->mName.C_Str(),
              static_cast<const void *>(node),
              static_cast<const void *>(node->mParent), node->mNumMeshes);

  for (unsigned i = 0; i < node->mNumChildren; ++i)
    dump_nodes(node->mChildren[i], depth + 1);
}

using BoneMap = std::unordered_map<std::string, u8>;

// add every bone to the skeleton
// and generate a name -> id mapping
void build_skel(const aiScene *scene, std::vector<Bone> &skeleton,
                std::vector<mat4> &local_transforms,
                std::vector<mat4> &inverse_binds, BoneMap &bone_map) {
  for (unsigned mesh_index = 0; mesh_index < scene->mNumMeshes; ++mesh_index) {
    const aiMesh *mesh = scene->mMeshes[mesh_index];
    for (unsigned i = 0; i < mesh->mNumBones; ++i) {
      const aiBone *bone = mesh->mBones[i];
      const char *name = bone->mName.C_Str();

      // DEBUG_PRINT("Bone in build: %s", name);

      // if this name was already added
      // just skip this to avoid duplicates
      if (bone_map.find(name) != bone_map.end())
        continue;
      u8 bone_id = skeleton.size();
      Bone out{};

      // NOTE: truncates name
      // TODO: better way for names?
      // to 3 chars (could have duplicates)
      out.name[0] = bone->mName.length > 0 ? bone->mName.data[0] : ' ';
      out.name[1] = bone->mName.length > 1 ? bone->mName.data[1] : ' ';
      out.name[2] = bone->mName.length > 2 ? bone->mName.data[2] : ' ';
      // NOTE: parent gets filled in later, once all bones are completely added
      // NOTE: for now, set its parent to itself
      out.parent = bone_id;

      // assigned name -> id mapping
      bone_map[name] = bone_id;

      skeleton.push_back(out);
      inverse_binds.push_back(convert_matrix(bone->mOffsetMatrix));
    }
  }
  local_transforms.resize(inverse_binds.size());
}

void build_parents_recurse(const aiNode *node,
                           std::vector<mat4> &local_transforms,
                           std::vector<Bone> &skeleton, BoneMap &bone_map) {
  auto it = bone_map.find(node->mName.C_Str());
  if (it != bone_map.end()) {
    u8 bone_id = it->second;
    Bone &bone = skeleton[bone_id];

    // NOTE: add on the local transform for the bone
    local_transforms[bone_id] = convert_matrix(node->mTransformation);

    // NOTE: here we use while because we want to
    // traverse until we reach an actual BONE parent node
    // not just any node
    // TODO: could be a for loop right?
    const aiNode *parent = node->mParent;
    while (parent) {
      auto parent_it = bone_map.find(parent->mName.C_Str());
      if (parent_it != bone_map.end()) {
        u8 parent_id = parent_it->second;
        bone.parent = parent_id;
        break;
      }

      parent = parent->mParent;
    }
  }

  // add on children as well
  for (usize i = 0; i < node->mNumChildren; ++i)
    build_parents_recurse(node->mChildren[i], local_transforms, skeleton,
                          bone_map);
}

// second pass:
// traverse from root node to
// build parent hierarchy for bones
void build_parents(const aiScene *scene, std::vector<mat4> &local_transforms,
                   std::vector<Bone> &skeleton, BoneMap &bone_map) {
  build_parents_recurse(scene->mRootNode, local_transforms, skeleton, bone_map);
}

void build_mesh(const aiScene *scene, LoadedMesh &result, BoneMap &bone_map) {
  for (unsigned mesh_index = 0; mesh_index < scene->mNumMeshes; ++mesh_index) {
    const aiMesh *mesh = scene->mMeshes[mesh_index];
    const u32 vertex_offset = result.vertices.size();

    // add vertices
    for (unsigned i = 0; i < mesh->mNumVertices; ++i) {
      Vertex vertex{};

      const aiVector3D &p = mesh->mVertices[i];
      vertex.position = {p.x, p.y, p.z};
      if (mesh->HasNormals()) {
        const aiVector3D &n = mesh->mNormals[i];
        vertex.normal = {n.x, n.y, n.z};
      }
      // NOTE: we get first texture coords
      if (mesh->HasTextureCoords(0)) {
        const aiVector3D &uv = mesh->mTextureCoords[0][i];
        vertex.uv = {uv.x, uv.y};
      }
      result.vertices.push_back(vertex);
    }

    // add indices
    for (unsigned i = 0; i < mesh->mNumFaces; ++i) {
      const aiFace &face = mesh->mFaces[i];
      if (face.mNumIndices != 3)
        continue;
      // NOTE: this offset is needed for indexing to work properly
      result.indices.push_back({
          vertex_offset + face.mIndices[0],
          vertex_offset + face.mIndices[1],
          vertex_offset + face.mIndices[2],
      });
    }

    // add bone weights
    // counts the number of influences tracked per-vertex
    // so we can properly add them
    std::vector<u8> influence_counts(mesh->mNumVertices);

    for (unsigned i = 0; i < mesh->mNumBones; ++i) {
      const aiBone *bone = mesh->mBones[i];
      u8 bone_id = bone_map[bone->mName.C_Str()];

      // now apply weights
      for (unsigned j = 0; j < bone->mNumWeights; ++j) {
        const aiVertexWeight &weight = bone->mWeights[j];
        // get vertex for weight
        auto vert_id = vertex_offset + weight.mVertexId;
        Vertex &vertex = result.vertices[vert_id];

        u8 &slot = influence_counts[weight.mVertexId];
        if (slot > 3)
          continue;
        vertex.bone_ids[slot] = bone_id;
        // range from 0 to 255
        vertex.bone_weights[slot] = (u8)(weight.mWeight * 255.0f);
        ++slot;
      }
    }
  }
}

void build_anims(const aiScene *scene, LoadedMesh &result, BoneMap &bone_map) {
  for (unsigned anim_index = 0; anim_index < scene->mNumAnimations;
       ++anim_index) {
    const aiAnimation *animation = scene->mAnimations[anim_index];
    // NOTE: should we use animation->mName anywehre?
    LoadedAnimation out_anim;
    float tps = animation->mTicksPerSecond;
    if (tps == 0) // fallback ticks per second
      tps = 25.0;
    out_anim.duration = animation->mDuration / tps;

    for (unsigned i = 0; i < animation->mNumChannels; ++i) {
      const aiNodeAnim *channel = animation->mChannels[i];
      LoadedBoneAnimation out_bone_anim;

      auto it = bone_map.find(channel->mNodeName.C_Str());
      if (it == bone_map.end())
        continue;

      out_bone_anim.id = bone_map.at(channel->mNodeName.C_Str());

      // add for each pos, rot, and scale
      for (unsigned j = 0; j < channel->mNumPositionKeys; ++j) {
        aiVectorKey key = channel->mPositionKeys[j];
        PositionKey pkey{(float)key.mTime / tps,
                         vec3{key.mValue.x, key.mValue.y, key.mValue.z}};
        out_bone_anim.positions.push_back(pkey);
      }
      for (unsigned j = 0; j < channel->mNumRotationKeys; ++j) {
        aiQuatKey key = channel->mRotationKeys[j];
        RotationKey rkey{
            (float)key.mTime / tps,
            quat{key.mValue.x, key.mValue.y, key.mValue.z, key.mValue.w}};
        out_bone_anim.rotations.push_back(rkey);
      }
      for (unsigned j = 0; j < channel->mNumScalingKeys; ++j) {
        aiVectorKey key = channel->mScalingKeys[j];
        ScaleKey skey{(float)key.mTime / tps,
                      vec3{key.mValue.x, key.mValue.y, key.mValue.z}};
        out_bone_anim.scales.push_back(skey);
      }

      out_anim.channels.push_back(out_bone_anim);
    }

    result.animations.push_back(out_anim);

    // animation->mNumChannels
    // animation->mChannels
  }
}
LoadedMesh assimp_load_scene(const aiScene *scene) {
  LoadedMesh result;
  BoneMap bone_map;

  // DEBUG PRINTING
  dump_nodes(scene->mRootNode, 0);

  // set up the skeleton array
  build_skel(scene, result.skeleton, result.local_transforms,
             result.inverse_binds, bone_map);
  // set up bone hierarchy
  build_parents(scene, result.local_transforms, result.skeleton, bone_map);

  // collapse meshes into one
  build_mesh(scene, result, bone_map);

  // for each animation
  build_anims(scene, result, bone_map);

  // TODO: textures / texture atlas
  // HACK: for now, just take the first texture we get bruh
  if (scene->mNumTextures > 0) {
    result.tex = convert_tex(scene->mTextures[0]);
  }

  return result;
}

LoadedMesh load_mesh(const char *path) {
  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile(
      path,
      aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

  if (!scene || !scene->HasMeshes())
    return {};

  return assimp_load_scene(scene);
}
