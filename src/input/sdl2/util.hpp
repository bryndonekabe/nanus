#pragma once
#include <SDL2/SDL.h>
#include <input/gamepad.hpp>
#include <input/keyboard.hpp>
#include <unordered_map>

using namespace nanus;
using namespace input;

#define _GAMEPAD_MAX_COUNT 16

inline Key sdl_key_table[SDL_NUM_SCANCODES];
inline void init_table() {
  sdl_key_table[SDL_SCANCODE_A] = Key::A;
  sdl_key_table[SDL_SCANCODE_B] = Key::B;
  sdl_key_table[SDL_SCANCODE_C] = Key::C;
  sdl_key_table[SDL_SCANCODE_D] = Key::D;
  sdl_key_table[SDL_SCANCODE_E] = Key::E;
  sdl_key_table[SDL_SCANCODE_F] = Key::F;
  sdl_key_table[SDL_SCANCODE_G] = Key::G;
  sdl_key_table[SDL_SCANCODE_H] = Key::H;
  sdl_key_table[SDL_SCANCODE_I] = Key::I;
  sdl_key_table[SDL_SCANCODE_J] = Key::J;
  sdl_key_table[SDL_SCANCODE_K] = Key::K;
  sdl_key_table[SDL_SCANCODE_L] = Key::L;
  sdl_key_table[SDL_SCANCODE_M] = Key::M;
  sdl_key_table[SDL_SCANCODE_N] = Key::N;
  sdl_key_table[SDL_SCANCODE_O] = Key::O;
  sdl_key_table[SDL_SCANCODE_P] = Key::P;
  sdl_key_table[SDL_SCANCODE_Q] = Key::Q;
  sdl_key_table[SDL_SCANCODE_R] = Key::R;
  sdl_key_table[SDL_SCANCODE_S] = Key::S;
  sdl_key_table[SDL_SCANCODE_T] = Key::T;
  sdl_key_table[SDL_SCANCODE_U] = Key::U;
  sdl_key_table[SDL_SCANCODE_V] = Key::V;
  sdl_key_table[SDL_SCANCODE_W] = Key::W;
  sdl_key_table[SDL_SCANCODE_X] = Key::X;
  sdl_key_table[SDL_SCANCODE_Y] = Key::Y;
  sdl_key_table[SDL_SCANCODE_Z] = Key::Z;

  sdl_key_table[SDL_SCANCODE_0] = Key::Num0;
  sdl_key_table[SDL_SCANCODE_1] = Key::Num1;
  sdl_key_table[SDL_SCANCODE_2] = Key::Num2;
  sdl_key_table[SDL_SCANCODE_3] = Key::Num3;
  sdl_key_table[SDL_SCANCODE_4] = Key::Num4;
  sdl_key_table[SDL_SCANCODE_5] = Key::Num5;
  sdl_key_table[SDL_SCANCODE_6] = Key::Num6;
  sdl_key_table[SDL_SCANCODE_7] = Key::Num7;
  sdl_key_table[SDL_SCANCODE_8] = Key::Num8;
  sdl_key_table[SDL_SCANCODE_9] = Key::Num9;

  sdl_key_table[SDL_SCANCODE_SPACE] = Key::Space;
  sdl_key_table[SDL_SCANCODE_TAB] = Key::Tab;
  sdl_key_table[SDL_SCANCODE_RETURN] = Key::Enter;
  sdl_key_table[SDL_SCANCODE_ESCAPE] = Key::Escape;
  sdl_key_table[SDL_SCANCODE_BACKSPACE] = Key::Backspace;

  sdl_key_table[SDL_SCANCODE_LSHIFT] = Key::LeftShift;
  sdl_key_table[SDL_SCANCODE_RSHIFT] = Key::RightShift;
  sdl_key_table[SDL_SCANCODE_LCTRL] = Key::LeftCtrl;
  sdl_key_table[SDL_SCANCODE_RCTRL] = Key::RightCtrl;
  sdl_key_table[SDL_SCANCODE_LALT] = Key::LeftAlt;
  sdl_key_table[SDL_SCANCODE_RALT] = Key::RightAlt;
  sdl_key_table[SDL_SCANCODE_LGUI] = Key::LeftSuper;
  sdl_key_table[SDL_SCANCODE_RGUI] = Key::RightSuper;

  sdl_key_table[SDL_SCANCODE_UP] = Key::Up;
  sdl_key_table[SDL_SCANCODE_DOWN] = Key::Down;
  sdl_key_table[SDL_SCANCODE_LEFT] = Key::Left;
  sdl_key_table[SDL_SCANCODE_RIGHT] = Key::Right;

  sdl_key_table[SDL_SCANCODE_INSERT] = Key::Insert;
  sdl_key_table[SDL_SCANCODE_DELETE] = Key::Delete;
  sdl_key_table[SDL_SCANCODE_HOME] = Key::Home;
  sdl_key_table[SDL_SCANCODE_END] = Key::End;
  sdl_key_table[SDL_SCANCODE_PAGEUP] = Key::PageUp;
  sdl_key_table[SDL_SCANCODE_PAGEDOWN] = Key::PageDown;

  sdl_key_table[SDL_SCANCODE_F1] = Key::F1;
  sdl_key_table[SDL_SCANCODE_F2] = Key::F2;
  sdl_key_table[SDL_SCANCODE_F3] = Key::F3;
  sdl_key_table[SDL_SCANCODE_F4] = Key::F4;
  sdl_key_table[SDL_SCANCODE_F5] = Key::F5;
  sdl_key_table[SDL_SCANCODE_F6] = Key::F6;
  sdl_key_table[SDL_SCANCODE_F7] = Key::F7;
  sdl_key_table[SDL_SCANCODE_F8] = Key::F8;
  sdl_key_table[SDL_SCANCODE_F9] = Key::F9;
  sdl_key_table[SDL_SCANCODE_F10] = Key::F10;
  sdl_key_table[SDL_SCANCODE_F11] = Key::F11;
  sdl_key_table[SDL_SCANCODE_F12] = Key::F12;
}

struct SDLController {
  SDL_GameController *controller;
  int port; // index into gamepads[]
};

inline std::unordered_map<SDL_JoystickID, SDLController> controllers;
inline SDL_GameController *ports[_GAMEPAD_MAX_COUNT];
inline Gamepad gamepads[_GAMEPAD_MAX_COUNT];

inline const unsigned char *sdl_keys;
inline int sdl_keys_len;
inline Keyboard kb{};
