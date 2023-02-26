#pragma once

#include <glad/glad.h>

#include "poggles/gltypes.h"
#include "poggles/poggles_export.hpp"

//------------------------------------------------------------------------------
// Typesafe OpenGL functions. Will only accept *_id classes from gltypes.h
//------------------------------------------------------------------------------

namespace poggles::gl
{
auto POGGLES_EXPORT bindBuffer(GLenum target, buffer_id buffer) -> void;

auto POGGLES_EXPORT bindVertexArray(vertex_array_id array) -> void;

auto POGGLES_EXPORT useProgram(program_id program) -> void;

auto POGGLES_EXPORT attachShader(program_id program, shader_id shader) -> void;

}  // namespace poggles::gl