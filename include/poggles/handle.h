#pragma once
#include <functional>
#include <stdexcept>

#include <glad/glad.h>
#include "poggles/poggles_export.hpp"

namespace poggles
{

//------------------------------------------------------------------------------
// An RAII wrapper around a resource that is not a pointer.
//
// Similar to a unique_ptr, it allows only move and no copies.
//
// Assumes that T has a default value, and that the destructor
// can be safely called on the default value.
//------------------------------------------------------------------------------

template<typename T>
class handle
{
public:
  using handle_destructor = std::function<void(T)>;

  template<typename D, typename C, typename... Args>
  handle(D&& destructor, C&& constructor, Args&&... args)
      : m_resource(constructor(std::forward<Args>(args)...))
      , m_destructor(destructor)
  {
  }

  virtual ~handle()
  {
    if (m_destructor) {
      m_destructor(m_resource);
    }
  }

  // Copying not allowed
  handle(const handle<T>& other) = delete;
  auto operator=(const handle<T>& other) -> handle<T>& = delete;

  // Moving allowed
  handle(handle<T>&& other) noexcept
      : m_resource(std::move(other.m_resource))
      , m_destructor(std::move(other.m_destructor))
  {
    other.m_resource = {};
    other.m_destructor = {};
  }

  auto operator=(handle<T>&& other) noexcept -> handle<T>&
  {
    m_destructor(m_resource);
    m_resource = std::move(other.m_resource);
    m_destructor = std::move(other.m_destructor);

    other.m_resource = {};
    other.m_destructor = {};
    return *this;
  }

  // Getters for the m_resource
  explicit operator T() const { return m_resource; }
  [[nodiscard]] auto value() const -> T { return m_resource; }

private:
  T m_resource;
  handle_destructor m_destructor;
};

//------------------------------------------------------------------------------
// OpenGL has various combinations of glCreate* and glDelete* function pairs
//
// The handle class allows us to use them in a generic way, but requires that we
// call glCreate* and provide it with the corresponding glDelete* reference.
//
// These classes do that for the various known types that we plan to use.
//------------------------------------------------------------------------------

class POGGLES_EXPORT gluint_handle : public handle<GLuint>
{
public:
  using destructor = std::function<void(GLuint)>;

  template<typename C, typename... Args>
  gluint_handle(destructor&& destructor, C&& constructor, Args&&... args)
      : handle(destructor, constructor, args...)
  {
    if (value() == 0) {
      throw std::runtime_error("Invalid gluint_handle");
    }
  }
};

class POGGLES_EXPORT shader_id : public gluint_handle
{
public:
  explicit shader_id(GLenum type)
      : gluint_handle(glDeleteShader, glCreateShader, type)
  {
  }
};

class POGGLES_EXPORT program_id : public gluint_handle
{
public:
  program_id()
      : gluint_handle(glDeleteProgram, glCreateProgram)
  {
  }
};

class POGGLES_EXPORT gen_delete_handle : public gluint_handle
{
public:
  template<typename D, typename G>
  gen_delete_handle(D&& delete_func, G&& gen_func)
      : gluint_handle(
          // Destructor
          [delete_func](GLuint other) { delete_func(1, &other); },

          // Constructor
          [gen_func]()
          {
            GLuint other {};
            gen_func(1, &other);
            return other;
          })
  {
  }
};

class POGGLES_EXPORT buffer_id : public gen_delete_handle
{
public:
  buffer_id()
      : gen_delete_handle(glDeleteBuffers, glGenBuffers)
  {
  }
};

class POGGLES_EXPORT vertex_array_id : public gen_delete_handle
{
public:
  vertex_array_id()
      : gen_delete_handle(glDeleteVertexArrays, glGenVertexArrays)
  {
  }
};

class POGGLES_EXPORT texture_id : public gen_delete_handle
{
public:
  texture_id()
      : gen_delete_handle(glDeleteTextures, glGenTextures)
  {
  }
};

}  // namespace poggles
