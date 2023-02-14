#include "poggles/buffer.h"

poggles::buffer::buffer(GLenum target)
    : m_target(target)
{
  glBindBuffer(target, static_cast<buffer_id>(m_buffer_handle));
}

poggles::buffer::buffer(GLenum target,
                        GLuint index,
                        GLint size,
                        GLenum data_type)
    : m_target(target)
{
  glBindBuffer(target, static_cast<buffer_id>(m_buffer_handle));
  glVertexAttribPointer(index, size, data_type, GL_FALSE, 0, (void*)nullptr);
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
  glBindBuffer(target, static_cast<buffer_id>(m_buffer_handle));
  for (GLuint index = start_index; index < start_index + num_indices; ++index) {
    std::ptrdiff_t offset = static_cast<std::ptrdiff_t>(0)
        + (index - start_index) * static_cast<GLuint>(stride) / num_indices;
    glVertexAttribPointer(index,
                          static_cast<GLint>(size_per_index),
                          data_type,
                          GL_FALSE,
                          stride,
                          (void*)(offset));
    glEnableVertexAttribArray(index);
    glVertexAttribDivisor(index, instances_per_advancement);
  }
}
