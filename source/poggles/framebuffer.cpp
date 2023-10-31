#include "poggles/framebuffer.h"

#include <iostream>

using namespace poggles;

auto poggles::use(framebuffer_handle const &framebuffer, GLenum target) -> void {
    glBindFramebuffer(target, framebuffer.value());
}

auto poggles::addTexture2D(
    framebuffer_handle const &framebuffer,
    GLenum attachment,
    texture const &tex
) -> bool {
    tex.bind();
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.value());
    // TODO: Make sure this correctly handles incompatible textures
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        attachment,
        tex.target(),
        static_cast<GLuint>(tex),
        0
    );  // Level might need to be changed

    bool status = true;

    GLenum FBO_STATUS = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (FBO_STATUS != GL_FRAMEBUFFER_COMPLETE) {
        status = false;
    }

    return status;
}

auto poggles::setDrawBuffers(
    framebuffer_handle const &framebuffer,
    std::vector<GLenum> const &attachments
) -> void {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer.value());
    glDrawBuffers(static_cast<GLsizei>(attachments.size()), &attachments[0]);
}

auto poggles::useDefaultFramebuffer() -> void { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
