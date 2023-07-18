#pragma once

#include <array>
#include <cstdint>
#include <span>
#include <vector>

#include "poggles/gl_function.h"
#include "poggles/handle.h"
#include "poggles/poggles_export.hpp"

namespace poggles {

// TODO Buffer class sets up vertex attribute pointer, but the attribute pointer
// is associated with the vertex array instead. If there isn't a vertex array
// bound (or the correct one bound) when the constructor is called these won't
// provide intended behavior.
// Additionally, it is valid to have different vertex attribute pointers
// associated with the same buffer (ie: A buffer used by multiple shader
// programs but using different attribute indices for each)
class POGGLES_EXPORT buffer {
  public:
    // No glVertexAttribPointer
    explicit buffer(GLenum target);
    // with glVertexAttribPointer
    buffer(GLenum target, GLuint index, GLint size, GLenum data_type);
    // with glVertexAttribPointer and glVertexAttribDivisor
    buffer(
        GLenum target,
        GLuint start_index,
        GLuint num_indices,
        GLuint size_per_index,
        GLsizei stride,
        GLuint instances_per_advancement,
        GLenum data_type
    );

    // Public interface
    auto bind() const -> void { gl::bindBuffer(m_target, m_buffer_handle.value()); }
    explicit operator buffer_id() const { return m_buffer_handle.value(); }
    template<typename T>
    auto data(std::span<T> const &data, GLenum usage) -> void {
        bind();
        glBufferData(m_target, static_cast<GLsizei>(sizeof(T) * data.size()), data.data(), usage);
    }
    template<typename Wrapper>
    auto data(Wrapper const &matrix, GLenum usage) -> void {
        bind();
        data(
            std::span<const typename Wrapper::value_type>(
                matrix.data(), static_cast<std::uint64_t>(matrix.size())
            ),
            usage
        );
    }

  private:
    GLenum m_target;
    buffer_handle m_buffer_handle;
};
}  // namespace poggles
