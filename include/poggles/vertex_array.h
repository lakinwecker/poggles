#pragma once

#include "poggles/gl_function.h"
#include "poggles/handle.h"
#include "poggles/poggles_export.hpp"

namespace poggles
{

class POGGLES_EXPORT vertex_array
{
public:
  vertex_array();

  // Public interface
  void bind() const { gl::bindVertexArray(m_array_handle.id()); }
  explicit operator vertex_array_id() const { return m_array_handle.id(); }
  // TODO: my linting tool says that unbind() can be static. It's right, it can
  // be static.
  //       My question is: Should it be static?
  void unbind() const { gl::bindVertexArray({}); }

private:
  vertex_array_handle m_array_handle;
};
}  // namespace poggles
