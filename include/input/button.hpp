#pragma once

namespace nanus::input {
struct Button {
  bool current : 1;
  bool previous : 1;
};
} // namespace nanus::input
