#include <stdexcept>

#include "poggles/texture.h"

#include "stb_image.h"

poggles::TextureFromData::TextureFromData(std::variant<float*, uint8_t*> _data,
                                          int _width,
                                          int _height,
                                          int _channels)
    : data(_data)
    , width(_width)
    , height(_height)
    , channels(_channels)
    , level(0)
{
}

poggles::TextureFromData::TextureFromData(std::variant<float*, uint8_t*> _data,
                                          int _width,
                                          int _height,
                                          int _channels,
                                          GLint _level,
                                          GLenum _target)
    : data(_data)
    , width(_width)
    , height(_height)
    , channels(_channels)
    , level(_level)
    , target(_target)
{
}

// https://www.cppstories.com/2018/09/visit-variants/
template<class... Ts>
struct overload : Ts...
{
  using Ts::operator()...;
};
template<class... Ts>
overload(Ts...) -> overload<Ts...>;

poggles::texture::texture(GLenum target)
    : m_original_target(target)
{
  bind();
}

void poggles::texture::load(poggles::TextureType type)
{
  std::visit(
      overload {
          [this](TextureFromFile const& textureFromFile)
          {
            int width = 0;
            int height = 0;
            int channels = 0;
            uint8_t* data = stbi_load(textureFromFile.filename.c_str(),
                                      &width,
                                      &height,
                                      &channels,
                                      0);
            if (data != nullptr) {
              // Call ourselves, but with the relevant data extracted from the
              // file
              TextureFromData textureFromData {
                  data,
                  width,
                  height,
                  channels,
                  textureFromFile.level,
                  textureFromFile.target.value_or(m_original_target)};
              load(textureFromData);
            } else {
              throw std::runtime_error("Failed to load texture");
            }
            stbi_image_free(data);
          },
          [this](TextureFromData const& textureFromData)
          {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            GLenum format = GL_RGB;
            if (textureFromData.channels == 1) {
              format = GL_RED;
            } else if (textureFromData.channels == 2) {
              format = GL_RG;
            } else if (textureFromData.channels == 3) {
              format = GL_RGB;
            } else if (textureFromData.channels == 4) {
              format = GL_RGBA;
            }
            GLenum target = textureFromData.target.value_or(m_original_target);

            std::visit(overload {[&](float* data)
                                 {
                                   glTexImage2D(target,
                                                textureFromData.level,
                                                static_cast<GLint>(format),
                                                textureFromData.width,
                                                textureFromData.height,
                                                0,
                                                format,
                                                GL_FLOAT,
                                                data);
                                 },
                                 [&](uint8_t* data)
                                 {
                                   glTexImage2D(target,
                                                textureFromData.level,
                                                static_cast<GLint>(format),
                                                textureFromData.width,
                                                textureFromData.height,
                                                0,
                                                format,
                                                GL_UNSIGNED_BYTE,
                                                data);
                                 }},
                       textureFromData.data);
          }},
      type);
}
