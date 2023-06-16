#pragma once

#include <array>
#include <string>
#include <vector>

#include "poggles/handle.h"
#include "poggles/poggles_export.hpp"

namespace poggles
{
class POGGLES_EXPORT texture
{
public:
  explicit texture(GLenum target);
  explicit operator GLuint() const
  {
    return static_cast<GLuint>(m_texture_handle.value());
  }
  static void activate(GLenum texture_num) { glActiveTexture(texture_num); }
  void bind(GLenum texture_num = GL_TEXTURE0) const
  {
    activate(texture_num);
    glBindTexture(m_original_target,
                  static_cast<GLuint>(m_texture_handle.value()));
  }

  inline auto target() const -> GLenum { return m_original_target; }

  // TODO more general form for these functions
  void load(GLenum target, std::string const& filename, GLint level = 0);
  void load(GLenum target,
            unsigned char*,
            int width,
            int height,
            int channels,
            GLint level = 0);
  void load(GLenum target,
            float*,
            int width,
            int height,
            int channels,
            GLint level = 0);

private:
  GLenum m_original_target;
  texture_handle m_texture_handle;
};
}  // namespace poggles
