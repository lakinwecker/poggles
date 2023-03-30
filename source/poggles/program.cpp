#include <iostream>
#include <vector>

#include "poggles/program.h"

#include "poggles/gl_function.h"

auto poggles::checkLinkSuccess(program_id identifier) -> bool
{
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
    std::initializer_list<std::pair<GLenum, std::string>> const& shaderFiles)
    -> bool
{
  bool status = true;

  for (auto [type, filename] : shaderFiles) {
    shader_handle shader(type);
    status &= compileShader(shader.value(), filename);
    gl::attachShader(program, shader.value());
    // shader_handle should be safe to go out of scope after being attached
  }

  glLinkProgram(program);

  return status & poggles::checkLinkSuccess(program);
}

poggles::program::program(
    std::initializer_list<std::pair<GLenum, std::string>> const& shaderFiles)
{
  if (!compileProgram(m_program_handle.value(), shaderFiles)) {
    throw poggles::shader_link_exception("Shaders did not link.");
  }
}

poggles::program::program(std::filesystem::path const& vertex_path,
                          std::filesystem::path const& fragment_path)
    : m_vertex_path(vertex_path)
    , m_fragment_path(fragment_path)
{
  if (!compileProgram(m_program_handle.value(),
                      {{GL_VERTEX_SHADER, vertex_path.string()},
                       {GL_FRAGMENT_SHADER, fragment_path.string()}}))
  {
    throw poggles::shader_link_exception("Shaders did not link.");
  }
}

auto poggles::program::recompile() -> bool
{
  try {
    // Try to create a new program
    poggles::program new_program(m_vertex_path, m_fragment_path);
    *this = std::move(new_program);
    return true;
  } catch (poggles::shader_compile_exception const&) {
    // spdlog::warn("[PROGRAM] falling back to previous version of shaders");
    return false;
  }
}

auto poggles::program::attach(shader_id id) -> void
{
  gl::attachShader(static_cast<program_id>(m_program_handle), id);
}

void poggles::program::use() const
{
  gl::useProgram(static_cast<program_id>(m_program_handle));
}

void poggles::program::set_bool(const std::string& name, bool value) const
{
  glUniform1i(glGetUniformLocation(
                  static_cast<GLuint>(m_program_handle.value()), name.c_str()),
              static_cast<int>(value));
}
void poggles::program::set_int(const std::string& name, int value) const
{
  glUniform1i(glGetUniformLocation(
                  static_cast<GLuint>(m_program_handle.value()), name.c_str()),
              value);
}
void poggles::program::set_texture_sampler(const std::string& name,
                                           GLint texture_num) const
{
  glUniform1i(glGetUniformLocation(
                  static_cast<GLuint>(m_program_handle.value()), name.c_str()),
              texture_num - GL_TEXTURE0);
}
void poggles::program::set_uint(const std::string& name, unsigned value) const
{
  glUniform1ui(glGetUniformLocation(
                   static_cast<GLuint>(m_program_handle.value()), name.c_str()),
               value);
}
void poggles::program::set_float(const std::string& name, float value) const
{
  glUniform1f(glGetUniformLocation(
                  static_cast<GLuint>(m_program_handle.value()), name.c_str()),
              value);
}

void poggles::program::set_vec2(const std::string& name,
                                std::span<const float, 2> value) const
{
  glUniform2fv(glGetUniformLocation(
                   static_cast<GLuint>(m_program_handle.value()), name.c_str()),
               1,
               value.data());
}

void poggles::program::set_vec3(const std::string& name,
                                std::span<const float, 3> value) const
{
  glUniform3fv(glGetUniformLocation(
                   static_cast<GLuint>(m_program_handle.value()), name.c_str()),
               1,
               value.data());
}

void poggles::program::set_vec4(const std::string& name,
                                std::span<const float, 4> value) const
{
  glUniform4fv(glGetUniformLocation(
                   static_cast<GLuint>(m_program_handle.value()), name.c_str()),
               1,
               value.data());
}
void poggles::program::set_mat4(
    const std::string& name, std::span<const float, 16> value) const  // NOLINT
{
  glUniformMatrix4fv(
      glGetUniformLocation(static_cast<GLuint>(m_program_handle.value()),
                           name.c_str()),
      1,
      GL_TRUE,
      value.data());
}

auto poggles::program::set_double(const std::string& name, double value) const
    -> void
{
  glUniform1d(glGetUniformLocation(m_program_handle.value(), name.c_str()),
              value);
}

auto poggles::program::set_dvec3(const std::string& name,
                                 std::span<const double, 3> value) const -> void
{
  glUniform3dv(glGetUniformLocation(m_program_handle.value(), name.c_str()),
               1,
               value.data());
}

auto poggles::program::set_dmat4(const std::string& name,
                                 std::span<const double, 16> value) const
    -> void
{
  glUniformMatrix4dv(
      glGetUniformLocation(m_program_handle.value(), name.c_str()),
      1,
      GL_TRUE,
      value.data());
}

void poggles::program::set_uvec2(const std::string& name,
                                 std::span<const std::uint32_t, 2> value) const
{
  glUniform2uiv(
      glGetUniformLocation(static_cast<GLuint>(m_program_handle.value()),
                           name.c_str()),
      1,
      value.data());
}

void poggles::program::set_uvec3(const std::string& name,
                                 std::span<const std::uint32_t, 3> value) const
{
  glUniform3uiv(
      glGetUniformLocation(static_cast<GLuint>(m_program_handle.value()),
                           name.c_str()),
      1,
      value.data());
}

void poggles::program::set_uvec4(const std::string& name,
                                 std::span<const std::uint32_t, 4> value) const
{
  glUniform4uiv(
      glGetUniformLocation(static_cast<GLuint>(m_program_handle.value()),
                           name.c_str()),
      1,
      value.data());
}
