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

  GLenum target;

private:
  texture_handle m_texture_handle;
};

auto loadFromFile(texture& tex,
                  std::filesystem::path const& filepath,
                  std::optional<GLenum> target = {},
                  GLint level = 0) -> bool;

auto loadFromData(texture& tex,
                  std::variant<float*, uint8_t*> data,
                  int width,
                  int height,
                  int channels,
                  std::optional<GLenum> target = {},
                  GLint level = 0) -> bool;
}  // namespace poggles