#pragma once

#include <concepts>

#include <glad/glad.h>

#include "poggles/poggles_export.hpp"

// clang-format off
template<typename T>
concept GLuintValued = requires(T a) {
  { a.id() } -> std::convertible_to<GLuint>;
};
// clang-format on

template<typename T>
class gltype
{
protected:
  T m_id;
  // Protected constructor so it can't be created directly (Should only be
  // created using handle class)
  explicit gltype(T in_id)
      : m_id(in_id)
  {
  }

public:
  // TODO if this is extended to GLint's as well, sometimes -1 is the invalid
  // case. Figure out how to handle that
  gltype()
      : m_id(0)
  {
  }

  gltype(gltype<T> const& other) = default;
  gltype(gltype<T>&&) noexcept = default;
  auto operator=(const gltype<T>&) -> gltype<T>& = default;
  auto operator=(gltype<T>&&) noexcept -> gltype<T>& = default;

  auto id() const -> T { return m_id; }
  operator T() const { return m_id; }
  operator bool() const { return m_id != 0; }
};

// Forward definition of handle
// Allows handle to construct gltypes
namespace poggles
{
template<typename T>
class handle;
}  // namespace poggles

// Macro for defining opengl id types
// clang-format off
#define POGGLES_GL_TYPE_CLASS(type_name)               \
class POGGLES_EXPORT type_name : public gltype<GLuint>                \
{                                                      \
public:                                                \
  type_name() = default;                               \
  friend class poggles::handle<type_name>;             \
protected:                                             \
  type_name(GLuint id) :gltype<GLuint>(id) {}          \
};
// clang-format on

//------------------------------------------------------------------------------
// Typed OpenGL id wrapper classes
//------------------------------------------------------------------------------

POGGLES_GL_TYPE_CLASS(program_id)
POGGLES_GL_TYPE_CLASS(shader_id)
POGGLES_GL_TYPE_CLASS(buffer_id)
POGGLES_GL_TYPE_CLASS(vertex_array_id)
// TODO Connect texture format to texture ID
POGGLES_GL_TYPE_CLASS(texture_id)
