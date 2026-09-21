#include "util.hpp"
#include <input/gamepad.hpp>
#include <platform/debug.hpp>

namespace nanus::input {
const Gamepad &gamepad(usize port) { return gamepads[port]; }
void rumble(usize port, f32 low, f32 high, f32 seconds) {
  SDL_GameController *ctrl = ports[port];
  if (SDL_GameControllerHasRumble(ctrl)) {
    SDL_GameControllerRumble(ctrl, (Uint16)(low * 65535.0f),
                             (Uint16)(high * 65535.0f),
                             (Uint32)(seconds * 1000.0f));
  }
}
} // namespace nanus::input
