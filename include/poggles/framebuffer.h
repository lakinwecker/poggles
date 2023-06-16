#pragma once

#include <map>
#include <memory>

#include "poggles/handle.h"
#include "poggles/texture.h"

namespace poggles
{

auto POGGLES_EXPORT useDefaultFramebuffer() -> void;

class POGGLES_EXPORT framebuffer
{
public:
  // GL_DRAW_FRAMEBUFFER, GL_READ_FRAMEBUFFER, GL_FRAMEBUFFER
  explicit framebuffer(GLenum target = GL_FRAMEBUFFER);
  inline explicit operator GLuint() const
  {
    return static_cast<GLuint>(m_framebuffer_handle.value());
  }

  auto use() -> void;

  // TODO: The order a color attachment is added is important, possibly work on
  // a better way of managing this?
  auto addTexture(GLenum attachment, texture const& tex) -> bool;

private:
  GLenum m_original_target;
  framebuffer_handle m_framebuffer_handle;

  // TODO: Shared pointer doesn't feel ideal here, consider other object
  // TODO: These should be linked together in some way
  std::vector<GLenum> m_drawBuffers;
};
}  // namespace poggles