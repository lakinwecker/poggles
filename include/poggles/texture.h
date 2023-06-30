#pragma once

#include <array>
#include <filesystem>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "poggles/handle.h"
#include "poggles/poggles_export.hpp"

namespace poggles
{
class POGGLES_EXPORT texture
{
public:
  explicit texture(GLenum in_target);

  explicit operator GLuint() const;

  static void activate(GLenum texture_num) { glActiveTexture(texture_num); }

  void bind(GLenum texture_num = GL_TEXTURE0) const;

  inline auto target() const -> GLenum { return m_target; }

private:
  GLenum const m_target;
  texture_handle m_texture_handle;
};

auto uploadFromFile(texture const& tex,
                    std::filesystem::path const& filepath,
                    GLint level = 0) -> bool;

auto uploadFromData(texture const& tex,
                    std::variant<float*, uint8_t*> data,
                    int width,
                    int height,
                    int channels,
                    GLint level = 0) -> bool;
}  // namespace poggles