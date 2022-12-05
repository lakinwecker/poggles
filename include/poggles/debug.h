#pragma once

#include <glad/glad.h>
#include "poggles/poggles_export.hpp"

namespace poggles
{
POGGLES_EXPORT auto gl_debug_output(GLenum source,
                                    GLenum type,
                                    GLuint identifier,
                                    GLenum severity,
                                    GLsizei,
                                    const GLchar* message,
                                    const void*) -> void;
}  // namespace poggles
