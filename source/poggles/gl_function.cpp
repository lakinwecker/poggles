#include "poggles/gl_function.h"

#include <glad/glad.h>

namespace poggles
{

void gl::bindBuffer(GLenum target, buffer_id buffer)
{
  glBindBuffer(target, buffer);
}

void gl::bindVertexArray(vertex_array_id array)
{
  glBindVertexArray(array);
}

void gl::useProgram(program_id program)
{
  glUseProgram(program);
}

void gl::attachShader(program_id program, shader_id shader)
{
  glAttachShader(program, shader);
}

}  // namespace poggles