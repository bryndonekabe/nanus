#pragma once
#include <bedrock/types.hpp>
#include <input/button.hpp>
namespace nanus::input {
enum class Key : u8 {
  Invalid = 0,
  // Letters
  A,
  B,
  C,
  D,
  E,
  F,
  G,
  H,
  I,
  J,
  K,
  L,
  M,
  N,
  O,
  P,
  Q,
  R,
  S,
  T,
  U,
  V,
  W,
  X,
  Y,
  Z,

  // Numbers
  Num0,
  Num1,
  Num2,
  Num3,
  Num4,
  Num5,
  Num6,
  Num7,
  Num8,
  Num9,

  // Function keys
  F1,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
  F9,
  F10,
  F11,
  F12,
  F13,
  F14,
  F15,
  F16,
  F17,
  F18,
  F19,
  F20,
  F21,
  F22,
  F23,
  F24,

  // Modifiers
  LeftShift,
  RightShift,
  LeftCtrl,
  RightCtrl,
  LeftAlt,
  RightAlt,
  LeftSuper,
  RightSuper,

  // Navigation
  Up,
  Down,
  Left,
  Right,
  Home,
  End,
  PageUp,
  PageDown,
  Insert,
  Delete,

  // Whitespace / editing
  Space,
  Tab,
  Enter,
  Escape,
  Backspace,

  // Lock keys
  CapsLock,
  NumLock,
  ScrollLock,

  // Punctuation
  Grave,        // `
  Minus,        // -
  Equal,        // =
  LeftBracket,  // [
  RightBracket, // ]
  Backslash,
  Semicolon,  // ;
  Apostrophe, // '
  Comma,      // ,
  Period,     // .
  Slash,      // /

  // Numpad
  Keypad0,
  Keypad1,
  Keypad2,
  Keypad3,
  Keypad4,
  Keypad5,
  Keypad6,
  Keypad7,
  Keypad8,
  Keypad9,
  KeypadDecimal,
  KeypadDivide,
  KeypadMultiply,
  KeypadSubtract,
  KeypadAdd,
  KeypadEnter,
  KeypadEqual,

  // Misc
  PrintScreen,
  Pause,
  Menu,

  COUNT,
};
struct Keyboard {
  Button keys[(usize)Key::COUNT];
};
const Keyboard &keyboard();
} // namespace nanus::input
