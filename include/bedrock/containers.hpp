#pragma once

#include <bedrock/types.hpp>
// NOTE: is it ok to include a level above?
#include <platform/debug.hpp>

namespace nanus {

template <typename T> struct View {
  View(T *p, usize l) : ptr(p), len(l) {}
  T *ptr;
  usize len;
};
template <typename T> struct Ok {
  Ok(const T &v) : val(v) {}
  Ok(T &&v) : val(static_cast<T &&>(v)) {}
  T val;
};
template <typename E> struct Err {
  Err(const E &v) : val(v) {}
  Err(E &&v) : val(static_cast<E &&>(v)) {}
  E val;
};
template <typename T, typename E> class Result {
  enum class Tag : byte { Ok, Err } tag;
  union {
    Ok<T> ok;
    Err<E> err;
  };

public:
  explicit Result(const T &v) : ok(v), tag(Tag::Ok) {}
  explicit Result(T &&v) : ok(static_cast<T &&>(v)), tag(Tag::Ok) {}
  explicit Result(const E &e) : err(e), tag(Tag::Err) {}
  explicit Result(E &&e) : err(static_cast<E &&>(e)), tag(Tag::Err) {}

  bool is_ok() { return tag == Tag::Ok; }
  T unwrap() {
    if (is_ok())
      return ok.val;
    FATAL("called unwrap() on 'Err' type");
  }
  T expect(const char *msg) {
    if (is_ok())
      return ok.val;
    FATAL(msg);
  }
  T unwrap_or(const T &&def) {
    if (!is_ok())
      return def;
    return ok.val;
  }
};
} // namespace nanus
