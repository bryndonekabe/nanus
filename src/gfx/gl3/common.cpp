#include "util.hpp"
#include <GL/glew.h>
#include <gfx/common.hpp>
#include <platform/debug.hpp>

void compile_shader(GLuint shader, const char *const *src, bool vertex) {
  glShaderSource(shader, 1, src, NULL);
  glCompileShader(shader);
  /* compilation error handling */
  int success;
  char info_log[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, info_log);
    if (vertex)
      ERROR("Vertex shader: %s", info_log);
    else
      ERROR("Frag shader: %s", info_log);
  }
}

void link_program(GLuint sp, GLuint vs, GLuint fs) {
  glAttachShader(sp, vs);
  glAttachShader(sp, fs);
  glLinkProgram(sp); // links the program
  /* error handling */
  int success;
  char info_log[512];
  glGetProgramiv(sp, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(sp, 512, NULL, info_log);
    ERROR("Linking shaders failed: %s", info_log);
  }
}

void setup_shaders() {
  // vertex shader source
  static const char *vs_source =
      "#version 330 core\n"
      "layout (location = 0) in vec3 pos;\n"
      "layout (location = 1) in vec3 normal;\n"
      "layout (location = 2) in vec2 uv;\n"
      "layout (location = 3) in uvec4 bone_ids;\n"
      "layout (location = 4) in vec4 bone_weights;\n"

      "uniform mat4 model;\n"
      "uniform mat4 view;\n"
      "uniform mat4 proj;\n"
      "uniform mat4 bones[256];\n"

      "out vec3 frag_pos;\n"
      "out vec3 frag_normal;\n"
      "out vec2 tex_coord;\n"

      "void main(){\n"

      "float total_weight = bone_weights.x + "
      "bone_weights.y + bone_weights.z + bone_weights.w;\n"
      "mat4 skin = mat4(1.0);\n"

      "if (total_weight > 0.0) {\n"
      "skin ="
      "   bone_weights.x * bones[bone_ids.x]"
      " + bone_weights.y * bones[bone_ids.y]"
      " + bone_weights.z * bones[bone_ids.z]"
      " + bone_weights.w * bones[bone_ids.w];\n"
      "}\n"

      "vec4 skinned_pos = skin * vec4(pos, 1.0);\n"
      "vec4 world_pos = model * skinned_pos;\n"

      "mat3 normal_matrix = transpose(inverse(mat3(model)));\n"
      "frag_normal = normal_matrix * normal;\n"
      "frag_pos = world_pos.xyz;\n"
      "tex_coord = uv;\n"

      "gl_Position = proj * view * world_pos;\n"
      "}\0";

  static const char *fs_source =
      "#version 330 core\n"
      "in vec3 frag_pos;\n"
      "in vec3 frag_normal;\n"
      "in vec2 tex_coord;\n"
      "\n"
      "uniform sampler2D atlas;\n"
      "uniform vec2 _tex_offset;\n"
      "uniform vec2 _tex_scale;\n"
      "\n"
      "out vec4 FragColor;\n"
      "\n"

      "struct Light {"
      "vec3 pos;"
      "vec3 color;"
      "float intensity;"
      "};"
      "uniform Light light;\n"
      "void main(){"
      "vec2 atlas_uv = tex_coord * _tex_scale + _tex_offset;"
      "vec4 color = texture(atlas, atlas_uv);"
      "vec3 N = normalize(frag_normal);"
      "vec3 L = normalize(light.pos - frag_pos);"
      "float diffuse = max(dot(N, L), 0.0);"
      "float ambient = 0.2;"
      "vec3 lighting = light.color * (ambient + light.intensity * diffuse);"
      "FragColor = vec4(color.rgb * lighting, color.a);"
      // "FragColor = color;"
      "}\0";
  /* Vertex Shader stuff */
  unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
  compile_shader(vs, &vs_source, true);

  /* Fragment shader stuff */ // same process as with vertex shader
  unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
  compile_shader(fs, &fs_source, false);

  /* Creating shader program */
  // Creating the program will link the vertex and fragment shaders to be used
  unsigned int sp = glCreateProgram();
  link_program(sp, vs, fs);

  // delete shaders after use
  glDeleteShader(vs);
  glDeleteShader(fs);

  glUseProgram(sp);
}

// templates
template <typename T>
void attrib_ptr(GLuint idx, GLint size, usize stride, usize ptr);
template <typename T>
void attrib_i_ptr(GLuint idx, GLint size, usize stride, usize ptr);

// specifications
template <>
void attrib_ptr<f32>(GLuint idx, GLint size, usize stride, usize ptr) {
  glVertexAttribPointer(idx, size, GL_FLOAT, GL_FALSE, stride,
                        (const void *)ptr);
  glEnableVertexAttribArray(idx);
}
template <>
void attrib_ptr<u8>(GLuint idx, GLint size, usize stride, usize ptr) {
  // NOTE: normalizes from 0 to 1
  glVertexAttribPointer(idx, size, GL_UNSIGNED_BYTE, GL_TRUE, stride,
                        (const void *)ptr);
  glEnableVertexAttribArray(idx);
}

template <>
void attrib_i_ptr<u8>(GLuint idx, GLint size, usize stride, usize ptr) {
  glVertexAttribIPointer(idx, size, GL_UNSIGNED_BYTE, stride,
                         (const void *)ptr);
  glEnableVertexAttribArray(idx);
}

// NOTE: size is number of members
#define VERTEX_ATTRIB(type, member, idx, size)                                 \
  attrib_ptr<type>(idx, size, sizeof(Vertex), offsetof(Vertex, member))
#define VERTEX_I_ATTRIB(type, member, idx, size)                               \
  attrib_i_ptr<type>(idx, size, sizeof(Vertex), offsetof(Vertex, member))

void vertex_attrib() {
  VERTEX_ATTRIB(f32, position, 0, 3);
  VERTEX_ATTRIB(f32, normal, 1, 3);
  VERTEX_ATTRIB(f32, uv, 2, 2);
  // NOTE: changed to u8 as well
  VERTEX_I_ATTRIB(u8, bone_ids, 3, 4);
  // NOTE: changed to be u8
  VERTEX_ATTRIB(u8, bone_weights, 4, 4);
}

#undef VERTEX_ATTRIB
#undef VERTEX_I_ATTRIB

namespace nanus::gfx {
bool init() {
  static bool has_init = false;
  if (has_init)
    return true;
  bool ok = false;

  if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
    ERROR("SDL video init err: %s", SDL_GetError());
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  window = SDL_CreateWindow("OpenGL Window", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, 800, 600,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
  ctx = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, ctx);

  glewExperimental = GL_TRUE; // enables core profile for glew

  GLenum err = glewInit();
  if (err == GLEW_OK) {
    ok = true;
    DEBUG_PRINT("OpenGL Version: %s", glGetString(GL_VERSION));
  } else {
    ERROR("GLEW: %s", glewGetErrorString(err));
  }

  if (ok) {
    has_init = true;
    DEBUG_PRINT("SDL video init");
  }

  // TODO: necessary?
  // int w, h;
  // SDL_GL_GetDrawableSize(window, &w, &h);
  // glViewport(0, 0, w, h);

  setup_shaders();

  vao.init();
  vertex_attrib();

  glActiveTexture(
      GL_TEXTURE0); // activate the texture unit first before binding texture
  atlas.init();

  glEnable(GL_DEPTH_TEST);

  return ok;
}
bool deinit() {
  static bool has_deinit = false;
  if (has_deinit)
    return true;
  bool ok = false;

  vao.deinit();

  SDL_GL_DeleteContext(ctx);
  SDL_DestroyWindow(window);
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
  ok = true;

  if (ok) {
    has_deinit = true;
    DEBUG_PRINT("SDL video deinit");
  }
  return ok;
}
void begin() {}
void clear_color(vec4 c) { glClearColor(c.r, c.g, c.b, c.a); }
void clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
void swap() { SDL_GL_SwapWindow(window); }

u32 width() {
  int w, h;
  SDL_GL_GetDrawableSize(window, &w, &h);
  return w;
}

u32 height() {
  int w, h;
  SDL_GL_GetDrawableSize(window, &w, &h);
  return h;
}
} // namespace nanus::gfx
