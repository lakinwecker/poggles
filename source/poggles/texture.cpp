#include <cassert>
#include <stdexcept>
#include <string>

#include "poggles/texture.h"

#include "stb_image.h"

poggles::texture::texture(GLenum target)
    : m_original_target(target)
{
  bind();
}

void poggles::texture::load(GLenum target,
                            std::string const& filename,
                            GLint level)
{
  // load and generate the texture
  int width = 0;
  int height = 0;
  int channels = 0;
  unsigned char* data =
      stbi_load(filename.c_str(), &width, &height, &channels, 0);
  if (data != nullptr) {
    load(target, data, width, height, channels, level);
  } else {
    throw std::runtime_error("Failed to load texture");
  }
  stbi_image_free(data);
}

void poggles::texture::load(GLenum target,
                            unsigned char* data,
                            int width,
                            int height,
                            int channels,
                            GLint level)
{
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

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(target,
               level,
               static_cast<GLint>(format),
               width,
               height,
               0,
               format,
               GL_UNSIGNED_BYTE,
               data);
}

void poggles::texture::load(GLenum target,
                            float* data,
                            int width,
                            int height,
                            int channels,
                            GLint level)
{
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
  glTexImage2D(target,
               level,
               static_cast<GLint>(format),
               width,
               height,
               0,
               format,
               GL_FLOAT,
               data);
  // glGenerateMipmap(target);
}
