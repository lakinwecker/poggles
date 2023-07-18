#pragma once

#include <map>
#include <memory>

#include "poggles/handle.h"
#include "poggles/texture.h"

namespace poggles {

auto POGGLES_EXPORT useDefaultFramebuffer() -> void;

auto POGGLES_EXPORT use(framebuffer_handle const &framebuffer, GLenum target = GL_FRAMEBUFFER)
    -> void;
auto POGGLES_EXPORT
addTexture2D(framebuffer_handle const &framebuffer, GLenum attachment, texture const &tex) -> bool;

auto POGGLES_EXPORT
setDrawBuffers(framebuffer_handle const &framebuffer, std::vector<GLenum> const &attachments)
    -> void;

}  // namespace poggles
