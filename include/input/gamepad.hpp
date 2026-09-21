#pragma once
#include <bedrock/types.hpp>
#include <bedrock/vec.hpp>
#include <input/button.hpp>

namespace nanus::input {
enum class GamepadButton : u8 {
  // Face buttons
  FaceUp,
  FaceDown,
  FaceLeft,
  FaceRight,

  // Dpad
  PadUp,
  PadDown,
  PadLeft,
  PadRight,

  // sticks
  StickLeft,
  StickRight,

  // other buttons
  Start,
  Select,
  Home,
  // aliases
  Menu = Start,
  Options = Start,
  Plus = Start,
  Back = Select,
  View = Select,
  Share = Select,
  Guide = Home,
  Xbox = Home,
  PS = Home,

  ShoulderLeft,
  ShoulderRight,
  COUNT
};
struct Gamepad {
  struct {
    vec3 accel; // m/s^2
    vec3 gyro;  // rad/s
  } motion;
  Button buttons[(usize)GamepadButton::COUNT];
  // in range [-1, 1] for both x and y axes
  vec2 left_stick;
  vec2 right_stick;
  // in range [0, 1] for both triggers
  f32 left_trigger;
  f32 right_trigger;
  bool connected;
};
const Gamepad &gamepad(usize port);
// low & high [0,1], seconds >= 0
void rumble(usize port, f32 low, f32 high, f32 seconds);
} // namespace nanus::input
