#include "util.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>
#include <input/common.hpp>
#include <platform/debug.hpp>

// NOTE: internal
Key translate(SDL_Scancode code) { return sdl_key_table[code]; }
int alloc_port() {
  for (int i = 0; i < _GAMEPAD_MAX_COUNT; ++i) {
    if (!gamepads[i].connected)
      return i;
  }
  return -1; // no room
}
float normalize_stick(Sint16 v) {
  if (v >= 0)
    return (float)v / 32767.0f;
  else
    return (float)v / 32768.0f;
}
float normalize_trigger(Sint16 v) { return (float)v / 32767.0f; }
void fill_gamepad(int port) {
  auto &gamepad = gamepads[port];
  if (!gamepad.connected)
    return;
  // SDL_GameController *sdl_pad = SDL_GameControllerFromPlayerIndex(port);
  SDL_GameController *sdl_pad = ports[port];
  // SDL_GameController *sdl_pad = controllers[port].controller;

  // 0 - 32767
  Sint16 sdl_ltrigger = SDL_GameControllerGetAxis(
      sdl_pad, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERLEFT);
  Sint16 sdl_rtrigger = SDL_GameControllerGetAxis(
      sdl_pad, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
  // -32768 - 32767
  Sint16 sdl_leftx = SDL_GameControllerGetAxis(
      sdl_pad, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX);
  Sint16 sdl_lefty = SDL_GameControllerGetAxis(
      sdl_pad, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY);
  Sint16 sdl_rightx = SDL_GameControllerGetAxis(
      sdl_pad, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX);
  Sint16 sdl_righty = SDL_GameControllerGetAxis(
      sdl_pad, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTY);
  // shoulder buttons
  bool sdl_shoulderleft = SDL_GameControllerGetButton(
      sdl_pad, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
  bool sdl_shoulderright = SDL_GameControllerGetButton(
      sdl_pad, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
  // face buttons
  bool sdl_facedown = SDL_GameControllerGetButton(
      sdl_pad, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A);
  bool sdl_faceup = SDL_GameControllerGetButton(
      sdl_pad, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y);
  bool sdl_faceleft = SDL_GameControllerGetButton(
      sdl_pad, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X);
  bool sdl_faceright = SDL_GameControllerGetButton(
      sdl_pad, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B);
  // dpad buttons
  bool sdl_padup = SDL_GameControllerGetButton(
      sdl_pad, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP);
  bool sdl_paddown = SDL_GameControllerGetButton(
      sdl_pad, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN);
  bool sdl_padleft = SDL_GameControllerGetButton(
      sdl_pad, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT);
  bool sdl_padright = SDL_GameControllerGetButton(
      sdl_pad, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
  // misc
  bool sdl_start = SDL_GameControllerGetButton(
      sdl_pad, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START);
  bool sdl_back = SDL_GameControllerGetButton(
      sdl_pad, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_BACK);
  bool sdl_guide = SDL_GameControllerGetButton(
      sdl_pad, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_GUIDE);
  // stick buttons
  bool sdl_stickleft = SDL_GameControllerGetButton(
      sdl_pad, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSTICK);
  bool sdl_stickright = SDL_GameControllerGetButton(
      sdl_pad, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSTICK);

  // fill pad
  gamepad.left_trigger = normalize_trigger(sdl_ltrigger);
  gamepad.right_trigger = normalize_trigger(sdl_rtrigger);

  gamepad.left_stick =
      vec2(normalize_stick(sdl_leftx), normalize_stick(sdl_lefty));
  gamepad.right_stick =
      vec2(normalize_stick(sdl_rightx), normalize_stick(sdl_righty));

  gamepad.buttons[(usize)GamepadButton::ShoulderLeft].current =
      sdl_shoulderleft;
  gamepad.buttons[(usize)GamepadButton::ShoulderRight].current =
      sdl_shoulderright;

  gamepad.buttons[(usize)GamepadButton::FaceDown].current = sdl_facedown;
  gamepad.buttons[(usize)GamepadButton::FaceUp].current = sdl_faceup;
  gamepad.buttons[(usize)GamepadButton::FaceLeft].current = sdl_faceleft;
  gamepad.buttons[(usize)GamepadButton::FaceRight].current = sdl_faceright;

  gamepad.buttons[(usize)GamepadButton::PadUp].current = sdl_padup;
  gamepad.buttons[(usize)GamepadButton::PadDown].current = sdl_paddown;
  gamepad.buttons[(usize)GamepadButton::PadLeft].current = sdl_padleft;
  gamepad.buttons[(usize)GamepadButton::PadRight].current = sdl_padright;

  gamepad.buttons[(usize)GamepadButton::Start].current = sdl_start;
  gamepad.buttons[(usize)GamepadButton::Back].current = sdl_back;
  gamepad.buttons[(usize)GamepadButton::Guide].current = sdl_guide;

  gamepad.buttons[(usize)GamepadButton::StickLeft].current = sdl_stickleft;
  gamepad.buttons[(usize)GamepadButton::StickRight].current = sdl_stickright;

  // motion
  float sdl_gyro[3]; // rad/s
  if (SDL_GameControllerHasSensor(sdl_pad, SDL_SENSOR_GYRO)) {
    SDL_GameControllerGetSensorData(sdl_pad, SDL_SENSOR_GYRO, sdl_gyro, 3);
    gamepad.motion.gyro = vec3(sdl_gyro[0], sdl_gyro[1], sdl_gyro[2]);
  } else {
    gamepad.motion.gyro = vec3(0);
  }

  float sdl_accel[3]; // m/s^2
  if (SDL_GameControllerHasSensor(sdl_pad, SDL_SENSOR_ACCEL)) {
    SDL_GameControllerGetSensorData(sdl_pad, SDL_SENSOR_ACCEL, sdl_accel, 3);
    gamepad.motion.accel = vec3(sdl_accel[0], sdl_accel[1], sdl_accel[2]);
  } else {
    gamepad.motion.accel = vec3(0);
  }
}
void cdevice_add(int id) {
  if (!SDL_IsGameController(id)) {
    DEBUG_PRINT("Non controller device add attempt");
    return;
  }
  SDL_GameController *ctrl = SDL_GameControllerOpen(id);
  if (!ctrl)
    return;
  DEBUG_PRINT("Controller: %s", SDL_GameControllerName(ctrl));

  SDL_Joystick *joy = SDL_GameControllerGetJoystick(ctrl);
  SDL_JoystickID instance_id = SDL_JoystickInstanceID(joy);

  if (controllers.count(instance_id)) {
    SDL_GameControllerClose(ctrl);
    return;
  }
  int port = alloc_port();
  if (port < 0) {
    SDL_GameControllerClose(ctrl);
    return;
  }
  DEBUG_PRINT("Device add attempt: %i", port);

  SDL_GameControllerSetPlayerIndex(ctrl, port);
  if (SDL_GameControllerHasSensor(ctrl, SDL_SENSOR_GYRO)) {
    SDL_GameControllerSetSensorEnabled(ctrl, SDL_SENSOR_GYRO, SDL_TRUE);
  }

  if (SDL_GameControllerHasSensor(ctrl, SDL_SENSOR_ACCEL)) {
    SDL_GameControllerSetSensorEnabled(ctrl, SDL_SENSOR_ACCEL, SDL_TRUE);
  }

  controllers[instance_id] = {
      .controller = ctrl,
      .port = port,
  };
  ports[port] = ctrl;
  gamepads[port].connected = true;
}
void cdevice_remove(int instance_id) {
  auto it = controllers.find(instance_id);
  if (it == controllers.end())
    return;
  SDL_GameController *ctrl = it->second.controller;
  int port = it->second.port;
  DEBUG_PRINT("Device remove attempt: %i", port);

  SDL_GameControllerSetPlayerIndex(ctrl, -1);
  SDL_GameControllerClose(ctrl);

  ports[port] = nullptr;
  gamepads[port] = {};

  controllers.erase(it);
}
void scan_controllers() {
  int n = SDL_NumJoysticks();
  DEBUG_PRINT("Num joysticks: %i", n);
  for (int i = 0; i < n; i++) {
    if (SDL_IsGameController(i))
      cdevice_add(i);
  }
}

namespace nanus::input {
bool init() {
  static bool has_init = false;
  if (has_init)
    return true;
  bool ok = false;

  DEBUG_PRINT("Controller init");
  if (SDL_InitSubSystem(SDL_INIT_EVENTS | SDL_INIT_JOYSTICK |
                        SDL_INIT_GAMECONTROLLER) != 0)
    ERROR("SDL event init err: %s", SDL_GetError());
  else {
    ok = true;
    SDL_GameControllerEventState(SDL_ENABLE);
    DEBUG_PRINT("Scanning controllers");
    scan_controllers();
  }

  sdl_keys = SDL_GetKeyboardState(&sdl_keys_len);
  init_table();
  // default initialize keyboard and gamepad
  // TODO: gamepad
  for (int i = 0; i < (int)Key::COUNT; ++i) {
    kb.keys[i].current = false;
    kb.keys[i].previous = false;
  }

  if (ok) {
    has_init = true;
  }

  return ok;
}

bool deinit() {
  static bool has_deinit = false;
  if (has_deinit)
    return true;

  for (auto &[id, ctrl] : controllers)
    SDL_GameControllerClose(ctrl.controller);
  controllers.clear();

  bool ok = false;
  SDL_QuitSubSystem(SDL_INIT_EVENTS | SDL_INIT_JOYSTICK |
                    SDL_INIT_GAMECONTROLLER);
  ok = true;

  if (ok) {
    has_deinit = true;
    DEBUG_PRINT("SDL input deinit");
  }
  return ok;
}

void poll() {
  // apply current state to previous
  for (int i = 0; i < (int)Key::COUNT; ++i) {
    kb.keys[i].previous = kb.keys[i].current;
  }
  for (int i = 0; i < _GAMEPAD_MAX_COUNT; ++i) {
    auto &pad = gamepads[i];
    for (int j = 0; j < (int)GamepadButton::COUNT; ++j) {
      pad.buttons[j].previous = pad.buttons[j].current;
    }
  }

  SDL_Event ev;
  while (SDL_PollEvent(&ev)) {
    // process events
    switch (ev.type) {
    case SDL_CONTROLLERDEVICEADDED: {
      DEBUG_PRINT("Device added");
      auto id = ev.cdevice.which;
      cdevice_add(id);
      break;
    }
    case SDL_CONTROLLERDEVICEREMOVED: {
      DEBUG_PRINT("Device removed");
      auto instance_id = ev.cdevice.which;
      cdevice_remove(instance_id);
      break;
    }
    }
  }

  // fill gamepads
  for (int i = 0; i < _GAMEPAD_MAX_COUNT; ++i) {
    auto port = i;
    fill_gamepad(port);
  }
  // fill keyboard
  for (int i = 0; i < sdl_keys_len; i++) {
    SDL_Scancode code = (SDL_Scancode)i;
    bool pressed = sdl_keys[code];
    Key translated_key = translate(code);
    if (translated_key != Key::Invalid)
      kb.keys[(usize)translated_key].current = pressed;
  }
}
} // namespace nanus::input
