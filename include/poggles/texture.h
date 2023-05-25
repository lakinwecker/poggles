#pragma once

#include <array>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "poggles/handle.h"
#include "poggles/poggles_export.hpp"

namespace poggles
{
struct TextureFromFile
{
  std::string filename;
  GLint level;
  std::optional<GLenum> target;
};

struct TextureFromData
{
  TextureFromData(std::variant<float*, uint8_t*> _data,
                  int _width,
                  int _height,
                  int _channels,
                  GLint _level,
                  GLenum _target);
  TextureFromData(std::variant<float*, uint8_t*> _data,
                  int _width,
                  int _height,
                  int _channels);

  std::variant<float*, uint8_t*> data;
  int width;
  int height;
  int channels;
  GLint level;
  std::optional<GLenum> target;
};

using TextureType = std::variant<TextureFromFile, TextureFromData>;

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

  void load(TextureType type);

private:
  GLenum m_original_target;
  texture_handle m_texture_handle;
};
}  // namespace poggles
