#pragma once
#include <functional>
#include <stdexcept>

#include <glad/glad.h>

#include "poggles/gltypes.h"
#include "poggles/poggles_export.hpp"

namespace poggles
{
// NOTES
// - Since underlying types are GLuint there's no type safety
// -

//------------------------------------------------------------------------------
// An RAII wrapper around a resource that is not a pointer.
//
// Similar to a unique_ptr, it allows only move and no copies.
//
// Assumes that T has a default value, and that the destructor
// can be safely called on the default value.
//------------------------------------------------------------------------------

// CONS of subclass
//    - Requires m_value member of subclass
//    - Handle becomes much less general
// PROS
//    - Easily allows for gltypes to be constructed only in this context (or a
//    similar means)
//    - Can convert directly to underlying type without needing to define
//    additional constructors

template<typename T>
class handle : public T
{
protected:
  using handle_destructor = std::function<void(T)>;

  template<typename D, typename C, typename... Args>
  handle(D&& destructor, C&& constructor, Args&&... args)
      : T(constructor(std::forward<Args>(args)...))
      , m_destructor(destructor)
  {
  }

public:
  virtual ~handle()
  {
    if (m_destructor) {
      m_destructor(T(*this));
    }
  }

  // Copying not allowed
  handle(const handle<T>& other) = delete;
  auto operator=(const handle<T>& other) -> handle<T>& = delete;

  // Moving allowed
  handle(handle<T>&& other) noexcept
      : T(std::move(static_cast<T&&>(other)))
      , m_destructor(std::move(other.m_destructor))
  {
    static_cast<T&>(*this) = {};
    other.m_destructor = {};
  }

  auto operator=(handle<T>&& other) noexcept -> handle<T>&
  {
    m_destructor(static_cast<T&>(*this));
    static_cast<T&&>(*this) = std::move(static_cast<T&&>(other));
    m_destructor = std::move(other.m_destructor);

    static_cast<T&>(other) = {};
    other.m_destructor = {};
    return *this;
  }

public:
  // Getters for the m_resource
  // explicit operator T() const { return m_value; }
  //[[nodiscard]] auto value() const -> T { return m_resource; }
  auto id() const -> T { return *this; }

private:
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
class POGGLES_EXPORT gluint_handle : public handle<T>
{
protected:
  using destructor = std::function<void(GLuint)>;

  template<typename C, typename... Args>
  gluint_handle(destructor&& _destructor, C&& _constructor, Args&&... args)
      : handle<T>(_destructor, _constructor, args...)
  {
    if (this->value() == 0) {
      throw std::runtime_error("Invalid gluint_handle");
    }
  }
};

class POGGLES_EXPORT shader_handle : public gluint_handle<shader_id>
{
public:
  explicit shader_handle(GLenum type)
      : gluint_handle<shader_id>(glDeleteShader, glCreateShader, type)
  {
  }
};

class POGGLES_EXPORT program_handle : public gluint_handle<program_id>
{
public:
  explicit program_handle()
      : gluint_handle<program_id>(glDeleteProgram, glCreateProgram)
  {
  }
};

template<typename T>
class POGGLES_EXPORT gen_delete_handle : public gluint_handle<T>
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

class POGGLES_EXPORT buffer_handle : public gen_delete_handle<buffer_id>
{
public:
  buffer_handle()
      : gen_delete_handle<buffer_id>(glDeleteBuffers, glGenBuffers)
  {
  }
};

class POGGLES_EXPORT vertex_array_handle
    : public gen_delete_handle<vertex_array_id>
{
public:
  vertex_array_handle()
      : gen_delete_handle<vertex_array_id>(glDeleteVertexArrays,
                                           glGenVertexArrays)
  {
  }
};

class POGGLES_EXPORT texture_handle : public gen_delete_handle<texture_id>
{
public:
  texture_handle()
      : gen_delete_handle<texture_id>(glDeleteTextures, glGenTextures)
  {
  }
};

}  // namespace poggles
