#include <stdexcept>

#include "poggles/texture.h"

#include "stb_image.h"

// https://www.cppstories.com/2018/09/visit-variants/
template<class... Ts>
struct overload : Ts...
{
  using Ts::operator()...;
};
template<class... Ts>
overload(Ts...) -> overload<Ts...>;

auto poggles::loadFromFile(poggles::texture& tex,
                           std::filesystem::path const& filename,
                           std::optional<GLenum> target,
                           GLint level) -> bool
{
  int width = 0;
  int height = 0;
  int channels = 0;
  uint8_t* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
  if (data != nullptr) {
    if (target.has_value()) {
      tex.target = *target;
    }
    // Call ourselves, but with the relevant data extracted from the
    // file
    bool status =
        loadFromData(tex, data, width, height, channels, target, level);
    stbi_image_free(data);
    return status;
  } else {
    return false;
  }
}

auto poggles::loadFromData(poggles::texture& tex,
                           std::variant<float*, uint8_t*> data,
                           int width,
                           int height,
                           int channels,
                           std::optional<GLenum> target,
                           GLint level) -> bool
{
  if (target.has_value()) {
    tex.target = *target;
  }

  glTexParameteri(tex.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(tex.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(tex.target, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(tex.target, GL_TEXTURE_WRAP_T, GL_REPEAT);

  GLenum format = GL_RGB;
  if (channels == 1) {
    format = GL_RED;
  } else if (channels == 2) {
    format = GL_RG;
  } else if (channels == 3) {
    format = GL_RGB;
  } else if (channels == 4) {
    format = GL_RGBA;
  }

  std::visit(overload {[&](float* pixelData)
                       {
                         glTexImage2D(tex.target,
                                      level,
                                      static_cast<GLint>(format),
                                      width,
                                      height,
                                      0,
                                      format,
                                      GL_FLOAT,
                                      pixelData);
                       },
                       [&](uint8_t* pixelData)
                       {
                         glTexImage2D(tex.target,
                                      level,
                                      static_cast<GLint>(format),
                                      width,
                                      height,
                                      0,
                                      format,
                                      GL_UNSIGNED_BYTE,
                                      pixelData);
                       }},
             data);
  return true;  // TODO: Consider how to handle errors better in poggles - this
                // should be false on a glError
}

poggles::texture::texture(GLenum in_target)
    : target(in_target)
{
  bind();
}

void poggles::texture::bind(GLenum texture_num) const
{
  activate(texture_num);
  glBindTexture(target, static_cast<GLuint>(m_texture_handle.value()));
}

poggles::texture::operator GLuint() const
{
  return static_cast<GLuint>(m_texture_handle.value());
}
