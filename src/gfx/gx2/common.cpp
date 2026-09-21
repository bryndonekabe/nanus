#include "util.hpp"
#include <gfx/common.hpp>
#include <gx2/clear.h>
#include <gx2/enum.h>
#include <gx2/state.h>
#include <platform/debug.hpp>
#include <stddef.h>
#include <whb/gfx.h>

namespace nanus::gfx {
bool init() {
  static bool has_init = false;
  if (has_init)
    return true;
  bool ok = false;

  if (WHBGfxInit() != TRUE)
    ERROR("WHB Gfx init err");
  else
    ok = true;

  GX2SetShaderModeEx(GX2_SHADER_MODE_UNIFORM_REGISTER, 48, 64, 0, 0, 200, 192);

  vao.init();

  vs_block = GX2GetVertexUniformBlock(vao.group.vertexShader, "uniforms");
  ps_block = GX2GetPixelUniformBlock(vao.group.pixelShader, "uniforms");

  if (ok) {
    has_init = true;
    DEBUG_PRINT("GX2 init");
  }

  // TODO: necessary?
  // int w, h;
  // SDL_GL_GetDrawableSize(window, &w, &h);
  // glViewport(0, 0, w, h);

  // setup_shaders();

  // vao.init();
  // vertex_attrib();

  // glActiveTexture(
  // GL_TEXTURE0); // activate the texture unit first before binding texture
  // atlas.init();

  // glEnable(GL_DEPTH_TEST);

  return ok;
}
bool deinit() {
  static bool has_deinit = false;
  if (has_deinit)
    return true;
  bool ok = false;

  WHBGfxShutdown();

  vao.deinit();

  ok = true;

  if (ok) {
    has_deinit = true;
    DEBUG_PRINT("GX2 deinit");
  }
  return ok;
}
void begin() { WHBGfxBeginRender(); }
void clear_color(vec4 c) { clear_col = c; }
void clear() {
  GX2ClearBuffersEx(WHBGfxGetDRCColourBuffer(), WHBGfxGetDRCDepthBuffer(),
                    clear_col.r, clear_col.g, clear_col.b, clear_col.a, 1.0f, 0,
                    GX2_CLEAR_FLAGS_DEPTH);
  GX2ClearBuffersEx(WHBGfxGetTVColourBuffer(), WHBGfxGetTVDepthBuffer(),
                    clear_col.r, clear_col.g, clear_col.b, clear_col.a, 1.0f, 0,
                    GX2_CLEAR_FLAGS_DEPTH);
}
void swap() { WHBGfxFinishRender(); }

u32 width() {}
u32 height() {}
} // namespace nanus::gfx
