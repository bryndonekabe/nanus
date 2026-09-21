#pragma once

#include <bedrock/vec.hpp>

namespace nanus::gfx {
bool init();
bool deinit();
// begin drawing
void begin();
// sets clear color
void clear_color(vec4 c);
// does clearing
void clear();
// end drawing & swap buffers (present to screen)
void swap();

u32 width();
u32 height();
} // namespace nanus::gfx
