#include "poggles/gl_function.h"

#include <glad/glad.h>

void poggles::gl::bindBuffer(GLenum target, poggles::: buffer_id buffer)
{
  glBindBuffer(target, buffer);
}

void poggles::gl::bindVertexArray(poggles::vertex_array_id array)
{
  glBindVertexArray(array);
}
