#pragma once

#include <concepts>

#include <glad/glad.h>

#include "poggles/poggles_export.hpp"

template<typename T>
concept GLuintValued = requires(T a) {
                         {
                           a.value()
                           } -> std::convertible_to<GLuint>;
                       };

template<typename T>
class handle;

template<typename T>
class gltype
{
protected:
  T m_value;
  // Protected constructor so it can't be created directly (Should only be
  // created using handle)
  gltype(T v)
      : m_value(v)
  {
  }

public:
  gltype()
      : m_value(0)
  {
  }

  gltype(gltype<T>& other) = default;
  gltype<T>& operator=(const gltype<T>& other) = default;

  // TODO Consider making it a member of the handle and trying to use friend
  friend class handle<gltype<T>>;

  T value() const { return m_value; }
  operator T() const { return m_value; }
};

// clang-format off
// Macro for defining opengl id types
#define POGGLES_GL_TYPE_CLASS(type_name, handle_name)               \
class POGGLES_EXPORT type_name : public gltype<GLuint> \
{                                                      \
public:                                                \
  type_name() = default;                               \
  friend class handle<type_name>;                      \
  type_name(type_name const & other): gltype<GLuint>(other.value()) {} \
protected:                                             \
  type_name(GLuint id) :gltype<GLuint>(id) {}          \
};
// clang-format on

// operator bool() { return m_value != 0; }
// TODO - Improve error messages when trying to cast a GLuint to another type?
// "This constructor is protected" might be good enough though

POGGLES_GL_TYPE_CLASS(program_id, program_handle)
POGGLES_GL_TYPE_CLASS(shader_id, shader_handle)
POGGLES_GL_TYPE_CLASS(buffer_id, buffer_handle)
POGGLES_GL_TYPE_CLASS(vertex_array_id, vertex_array_handle)
POGGLES_GL_TYPE_CLASS(texture_id, texture_handle)

struct GLEnum
{
  // TODO - Actually make this
};
