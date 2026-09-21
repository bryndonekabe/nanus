#pragma once

// yes, these are libc headers, but theyre just typedef standards so we're not
// linking any api functions or anything
#include <stddef.h>
#include <stdint.h>

// TODO: vector types, matrices, quaternions
// TODO: more fixed width types
namespace nanus {
using i8 = int8_t;
using u8 = uint8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;

using usize = size_t;
using byte = u8;

using f32 = float;
using f64 = double;
static_assert(sizeof(f32) == 4, "Float must be 32 bits");
static_assert(sizeof(f64) == 8, "Double must be 64 bits");
}; // namespace nanus
