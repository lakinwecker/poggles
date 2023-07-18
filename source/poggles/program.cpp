#include <iostream>
#include <vector>

#include "poggles/program.h"

#include "poggles/gl_function.h"
#include "poggles/shader.h"

auto poggles::checkLinkSuccess(program_id identifier) -> bool {
    GLint success = -1;

    // check for link errors
    glGetProgramiv(identifier, GL_LINK_STATUS, &success);
    if (success == 0) {
        GLint log_length = -1;
        glGetProgramiv(identifier, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<char> log(static_cast<size_t>(log_length));
        glGetProgramInfoLog(identifier, log_length, nullptr, log.data());

        std::cout << "[PROGRAM] linking failed: " << log.data();

        return false;
    }

    return true;
}

auto poggles::compileProgram(
    program_id program,
    std::vector<shader_desc> const &shaderDescs,
    std::vector<std::string> const &defines
) -> bool {
    bool status = true;

    for (auto shaderDesc : shaderDescs) {
        shader_handle shader(shaderDesc.type);
        status &= compileShader(shader.value(), shaderDesc, defines);
        gl::attachShader(program, shader.value());
        // shader_handle should be safe to go out of scope after being attached
    }

    glLinkProgram(program);

    return status & poggles::checkLinkSuccess(program);
}

poggles::program::program(
    std::vector<shader_desc> const &shaderDescs,
    std::vector<std::string> const &defines
) {
    if (!compileProgram(m_program_handle.value(), shaderDescs, defines)) {
        throw poggles::shader_link_exception("Shaders did not link.");
    }
}

poggles::program::program(
    std::filesystem::path const &vertex_path,
    std::filesystem::path const &fragment_path,
    std::vector<std::string> const &defines
)
    : m_vertex_path(vertex_path)
    , m_fragment_path(fragment_path) {
    std::vector<shader_desc> shaderDescs
        = {poggles::shaderDescFromFile(GL_VERTEX_SHADER, vertex_path),
           poggles::shaderDescFromFile(GL_FRAGMENT_SHADER, fragment_path)};

    if (!compileProgram(m_program_handle.value(), shaderDescs, defines)) {
        throw poggles::shader_link_exception("Shaders did not link.");
    }
}

auto poggles::program::recompile() -> bool {
    try {
        // Try to create a new program
        poggles::program new_program(m_vertex_path, m_fragment_path);
        *this = std::move(new_program);
        return true;
    } catch (poggles::shader_compile_exception const &) {
        // spdlog::warn("[PROGRAM] falling back to previous version of shaders");
        return false;
    }
}

auto poggles::program::attach(shader_id id) -> void {
    gl::attachShader(static_cast<program_id>(m_program_handle), id);
}

void poggles::program::use() const { gl::useProgram(static_cast<program_id>(m_program_handle)); }

void poggles::program::set_bool(std::string const &name, bool value) const {
    glUniform1i(
        glGetUniformLocation(static_cast<GLuint>(m_program_handle.value()), name.c_str()),
        static_cast<int>(value)
    );
}
void poggles::program::set_int(std::string const &name, int value) const {
    glUniform1i(
        glGetUniformLocation(static_cast<GLuint>(m_program_handle.value()), name.c_str()), value
    );
}
void poggles::program::set_texture_sampler(std::string const &name, GLint texture_num) const {
    glUniform1i(
        glGetUniformLocation(static_cast<GLuint>(m_program_handle.value()), name.c_str()),
        texture_num - GL_TEXTURE0
    );
}
void poggles::program::set_uint(std::string const &name, unsigned value) const {
    glUniform1ui(
        glGetUniformLocation(static_cast<GLuint>(m_program_handle.value()), name.c_str()), value
    );
}
void poggles::program::set_float(std::string const &name, float value) const {
    glUniform1f(
        glGetUniformLocation(static_cast<GLuint>(m_program_handle.value()), name.c_str()), value
    );
}

void poggles::program::set_vec2(std::string const &name, std::span<float const, 2> value) const {
    glUniform2fv(
        glGetUniformLocation(static_cast<GLuint>(m_program_handle.value()), name.c_str()),
        1,
        value.data()
    );
}

void poggles::program::set_vec3(std::string const &name, std::span<float const, 3> value) const {
    glUniform3fv(
        glGetUniformLocation(static_cast<GLuint>(m_program_handle.value()), name.c_str()),
        1,
        value.data()
    );
}

void poggles::program::set_vec4(std::string const &name, std::span<float const, 4> value) const {
    glUniform4fv(
        glGetUniformLocation(static_cast<GLuint>(m_program_handle.value()), name.c_str()),
        1,
        value.data()
    );
}
void poggles::program::set_mat4(std::string const &name, std::span<float const, 16> value)
    const  // NOLINT
{
    glUniformMatrix4fv(
        glGetUniformLocation(static_cast<GLuint>(m_program_handle.value()), name.c_str()),
        1,
        GL_TRUE,
        value.data()
    );
}

auto poggles::program::set_double(std::string const &name, double value) const -> void {
    glUniform1d(glGetUniformLocation(m_program_handle.value(), name.c_str()), value);
}

auto poggles::program::set_dvec3(std::string const &name, std::span<double const, 3> value) const
    -> void {
    glUniform3dv(glGetUniformLocation(m_program_handle.value(), name.c_str()), 1, value.data());
}

auto poggles::program::set_dmat4(std::string const &name, std::span<double const, 16> value) const
    -> void {
    glUniformMatrix4dv(
        glGetUniformLocation(m_program_handle.value(), name.c_str()), 1, GL_TRUE, value.data()
    );
}

void poggles::program::set_uvec2(std::string const &name, std::span<const std::uint32_t, 2> value)
    const {
    glUniform2uiv(
        glGetUniformLocation(static_cast<GLuint>(m_program_handle.value()), name.c_str()),
        1,
        value.data()
    );
}

void poggles::program::set_uvec3(std::string const &name, std::span<const std::uint32_t, 3> value)
    const {
    glUniform3uiv(
        glGetUniformLocation(static_cast<GLuint>(m_program_handle.value()), name.c_str()),
        1,
        value.data()
    );
}

void poggles::program::set_uvec4(std::string const &name, std::span<const std::uint32_t, 4> value)
    const {
    glUniform4uiv(
        glGetUniformLocation(static_cast<GLuint>(m_program_handle.value()), name.c_str()),
        1,
        value.data()
    );
}
