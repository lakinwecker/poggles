#pragma once

#include "poggles/handle.h"
#include "poggles/poggles_export.hpp"

namespace poggles
{

class POGGLES_EXPORT vertex_array
{
public:
  vertex_array();

  // Public interface
  void bind() const { glBindVertexArray(static_cast<GLuint>(m_array_id)); }
  explicit operator GLuint() const { return static_cast<GLuint>(m_array_id); }
  // TODO: my linting tool says that unbind() can be static. It's right, it can
  // be static.
  //       My question is: Should it be static?
  void unbind() const { glBindVertexArray(0); }

private:
  vertex_array_id m_array_id;
};
}  // namespace poggles
