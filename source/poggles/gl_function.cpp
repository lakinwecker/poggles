#include <glad/glad.h>

#include "poggles/gl_function.h"

namespace poggles {

auto gl::bindBuffer(GLenum target, buffer_id buffer) -> void { glBindBuffer(target, buffer); }

auto gl::bindVertexArray(vertex_array_id array) -> void { glBindVertexArray(array); }

auto gl::useProgram(program_id program) -> void { glUseProgram(program); }

auto gl::attachShader(program_id program, shader_id shader) -> void {
    glAttachShader(program, shader);
}

}  // namespace poggles
