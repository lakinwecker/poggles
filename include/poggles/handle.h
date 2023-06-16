#pragma once
#include <functional>
#include <stdexcept>

#include <glad/glad.h>

#include "poggles/gltypes.h"
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
  handle(D&& _destructor, C&& _constructor, Args&&... args)
      : m_resource(_constructor(std::forward<Args>(args)...))
      , m_destructor(_destructor)
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
  [[nodiscard]] auto value() const -> T { return m_resource; }
  explicit operator T() const { return m_resource; }

private:
  T m_resource;
  handle_destructor m_destructor;
};

//------------------------------------------------------------------------------
// OpenGL has various combinations of glCreate* and glDelete* function pairs
//
// The handle class allows us to use them in a generic way, but requires that
// we call glCreate* and provide it with the corresponding glDelete*
// reference.
//
// These classes do that for the various known types that we plan to use.
//------------------------------------------------------------------------------

template<GLuintValued T>
class gluint_handle : public handle<T>
{
public:
  using destructor = std::function<void(GLuint)>;

  template<typename C, typename... Args>
  gluint_handle(destructor&& _destructor, C&& _constructor, Args&&... args)
      : handle<T>(_destructor, _constructor, args...)
  {
    if (this->value().id() == 0) {
      throw std::runtime_error("Invalid gluint_handle");
    }
  }

  explicit operator GLuint() const { return this->value().id(); }
};

class shader_handle : public gluint_handle<shader_id>
{
public:
  explicit shader_handle(GLenum type)
      : gluint_handle<shader_id>(glDeleteShader, glCreateShader, type)
  {
  }
};

class program_handle : public gluint_handle<program_id>
{
public:
  explicit program_handle()
      : gluint_handle<program_id>(glDeleteProgram, glCreateProgram)
  {
  }
};

template<typename T>
class gen_delete_handle : public gluint_handle<T>
{
public:
  template<typename D, typename G>
  gen_delete_handle(D&& delete_func, G&& gen_func)
      : gluint_handle<T>(
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

class buffer_handle : public gen_delete_handle<buffer_id>
{
public:
  buffer_handle()
      : gen_delete_handle<buffer_id>(glDeleteBuffers, glGenBuffers)
  {
  }
};

class vertex_array_handle : public gen_delete_handle<vertex_array_id>
{
public:
  vertex_array_handle()
      : gen_delete_handle<vertex_array_id>(glDeleteVertexArrays,
                                           glGenVertexArrays)
  {
  }
};

class texture_handle : public gen_delete_handle<texture_id>
{
public:
  texture_handle()
      : gen_delete_handle<texture_id>(glDeleteTextures, glGenTextures)
  {
  }
};

class framebuffer_handle : public gen_delete_handle<framebuffer_id>
{
public:
  framebuffer_handle()
      : gen_delete_handle<framebuffer_id>(glDeleteFramebuffers,
                                          glGenFramebuffers)
  {
  }
};

}  // namespace poggles
