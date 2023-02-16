#pragma once

#include <glad/glad.h>

#include "poggles/gltypes.h"
#include "poggles/poggles_export.hpp"

//------------------------------------------------------------------------------
// Typesafe OpenGL functions. Will only accept *_id classes from gltypes.h
//------------------------------------------------------------------------------

namespace poggles::gl
{
void POGGLES_EXPORT bindBuffer(GLenum target, buffer_id buffer);

void POGGLES_EXPORT bindVertexArray(vertex_array_id array);

void POGGLES_EXPORT useProgram(program_id program);

void POGGLES_EXPORT attachShader(program_id program, shader_id shader);

}  // namespace poggles::gl