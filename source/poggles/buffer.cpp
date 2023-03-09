#include "poggles/buffer.h"

// TODO vertex attribute indices may reserve subsequent indices dependent on the
// size of the data in the attribute. We need a better system for making sure
// reserved indices aren't specified
// I think we could remove this class and make free functions instead, similar
// to the program class
poggles::buffer::buffer(GLenum target)
    : m_target(target)
{
  gl::bindBuffer(target, static_cast<buffer_id>(m_buffer_handle));
}

poggles::buffer::buffer(GLenum target,
                        GLuint index,
                        GLint size,
                        GLenum data_type)
    : m_target(target)
{
  gl::bindBuffer(target, static_cast<buffer_id>(m_buffer_handle));
<<<<<<< HEAD
  if (data_type == GL_DOUBLE) {
    glVertexAttribLPointer(
        index, size, data_type, 0, static_cast<void*>(nullptr));
  } else {
    glVertexAttribPointer(
        index, size, data_type, GL_FALSE, 0, static_cast<void*>(nullptr));
  }
=======
  glVertexAttribPointer(
      index, size, data_type, GL_FALSE, 0, static_cast<void*>(nullptr));
>>>>>>> upstream/main
  glEnableVertexAttribArray(index);
}

poggles::buffer::buffer(GLenum target,
                        GLuint start_index,
                        GLuint num_indices,
                        GLuint size_per_index,
                        GLsizei stride,
                        GLuint instances_per_advancement,
                        GLenum data_type)
    : m_target(target)
{
  gl::bindBuffer(target, static_cast<buffer_id>(m_buffer_handle));
  for (GLuint index = start_index; index < start_index + num_indices; ++index) {
    std::ptrdiff_t offset = static_cast<std::ptrdiff_t>(0)
        + (index - start_index) * static_cast<GLuint>(stride) / num_indices;
    if (data_type == GL_DOUBLE) {
      glVertexAttribLPointer(index,
                             static_cast<GLint>(size_per_index),
                             data_type,
                             stride,
                             reinterpret_cast<void*>(offset));  // NOLINT
    } else {
      glVertexAttribPointer(index,
                            static_cast<GLint>(size_per_index),
                            data_type,
                            GL_FALSE,
                            stride,
                            reinterpret_cast<void*>(offset));  // NOLINT
    }
    glEnableVertexAttribArray(index);
    glVertexAttribDivisor(index, instances_per_advancement);
  }
}
