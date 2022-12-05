#pragma once

#include <array>
#include <cstdint>
#include <span>
#include <vector>

#include "poggles/handle.h"
#include "poggles/poggles_export.hpp"

namespace poggles
{

class POGGLES_EXPORT buffer
{
public:
  // No glVertexAttribPointer
  explicit buffer(GLenum target);
  // with glVertexAttribPointer
  buffer(GLenum target, GLuint index, GLint size, GLenum data_type);
  // with glVertexAttribPointer and glVertexAttribDivisor
  buffer(GLenum target,
         GLuint start_index,
         GLuint num_indices,
         GLuint size_per_index,
         GLsizei stride,
         GLuint instances_per_advancement,
         GLenum data_type);

  // Public interface
  auto bind() -> void
  {
    glBindBuffer(m_target, static_cast<GLuint>(m_buffer_id));
  }
  explicit operator GLuint() const { return static_cast<GLuint>(m_buffer_id); }
  template<typename T>
  auto data(const std::span<T>& data, GLenum usage) -> void
  {
    bind();
    glBufferData(
        m_target, static_cast<GLsizei>(sizeof(T) * data.size()), data.data(), usage);
  }
  template<typename Wrapper>
  auto data(Wrapper const& matrix, GLenum usage) -> void
  {
    bind();
    data(std::span<const typename Wrapper::value_type>(
             matrix.data(), static_cast<std::uint64_t>(matrix.size())),
         usage);
  }

private:
  GLenum m_target;
  buffer_id m_buffer_id;
};
}  // namespace poggles
