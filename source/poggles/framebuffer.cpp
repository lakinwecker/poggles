#include <iostream>

#include "poggles/framebuffer.h"

auto poggles::useDefaultFramebuffer() -> void
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

poggles::framebuffer::framebuffer(GLenum target)
    : m_original_target(target)
{
  glBindFramebuffer(m_original_target, m_framebuffer_handle.value());
}

auto poggles::framebuffer::use() -> void
{
  glBindFramebuffer(m_original_target, m_framebuffer_handle.value());
}

auto poggles::framebuffer::addTexture(GLenum attachment,
                                      poggles::texture const& tex) -> bool
{
  tex.bind();
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_handle.value());
  // TODO: Make sure this correctly handles incompatible textures
  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         attachment,
                         tex.target(),
                         static_cast<GLuint>(tex),
                         0);  // Level might need to be changed

  bool status = true;

  GLenum FBO_STATUS = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (FBO_STATUS != GL_FRAMEBUFFER_COMPLETE) {
    status = false;

    switch (FBO_STATUS) {
      case GL_FRAMEBUFFER_UNDEFINED:
        std::cout << "GL_FRAMEBUFFER_UNDEFINED" << std::endl;
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << std::endl;
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"
                  << std::endl;
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        std::cout << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" << std::endl;
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        std::cout << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" << std::endl;
        break;
      case GL_FRAMEBUFFER_UNSUPPORTED:
        std::cout << "GL_FRAMEBUFFER_UNSUPPORTED" << std::endl;
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" << std::endl;
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
        std::cout << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS" << std::endl;
        break;
    }
  }
  if (status && attachment != GL_DEPTH_ATTACHMENT) {
    m_drawBuffers.push_back(attachment);
    glDrawBuffers(static_cast<GLsizei>(m_drawBuffers.size()),
                  &m_drawBuffers[0]);
  }

  // TODO: Should recreate original state
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  return status;
}
