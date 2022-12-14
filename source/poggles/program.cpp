#include <iostream>
#include <vector>

#include "poggles/program.h"

poggles::program::program(std::filesystem::path const& vertex_path,
                          std::filesystem::path const& fragment_path)
    : m_vertex(vertex_path, GL_VERTEX_SHADER)
    , m_fragment(fragment_path, GL_FRAGMENT_SHADER)
{
  attach(static_cast<GLuint>(m_vertex));
  attach(static_cast<GLuint>(m_fragment));
  glLinkProgram(static_cast<GLuint>(m_program_id));

  if (!check_link_success(static_cast<GLuint>(m_program_id))) {
    throw poggles::shader_link_exception("Shaders did not link.");
  }
}

auto poggles::program::recompile() -> bool
{
  try {
    // Try to create a new program
    poggles::program new_program(m_vertex.get_path(), m_fragment.get_path());
    *this = std::move(new_program);
    return true;
  } catch (poggles::shader_compile_exception const&) {
    // spdlog::warn("[PROGRAM] falling back to previous version of shaders");
    return false;
  }
}

void poggles::program::use() const
{
  glUseProgram(static_cast<GLuint>(m_program_id));
}

auto poggles::program::check_link_success(GLuint identifier) -> bool
{
  GLint success = -1;

  // check for link errors
  glGetProgramiv(identifier, GL_LINK_STATUS, &success);
  if (success == 0) {
    GLint log_length = -1;
    glGetProgramiv(identifier, GL_INFO_LOG_LENGTH, &log_length);
    std::vector<char> log(static_cast<size_t>(log_length));
    glGetProgramInfoLog(identifier, log_length, nullptr, log.data());

    // spdlog::error("[PROGRAM] linking {} + {}:\n{}", vertex.get_path(),
    // fragment.get_path(), log.data());

    return false;
  }

  // spdlog::info("[PROGRAM] successfully compiled and linked {} + {}",
  // vertex.get_path(), fragment.get_path());
  return true;
}

void poggles::program::set_bool(const std::string& name, bool value) const
{
  glUniform1i(
      glGetUniformLocation(static_cast<GLuint>(m_program_id), name.c_str()),
      static_cast<int>(value));
}
void poggles::program::set_int(const std::string& name, int value) const
{
  glUniform1i(
      glGetUniformLocation(static_cast<GLuint>(m_program_id), name.c_str()),
      value);
}
void poggles::program::set_texture_sampler(const std::string& name,
                                           GLint texture_num) const
{
  glUniform1i(
      glGetUniformLocation(static_cast<GLuint>(m_program_id), name.c_str()),
      texture_num - GL_TEXTURE0);
}
void poggles::program::set_uint(const std::string& name, unsigned value) const
{
  glUniform1ui(
      glGetUniformLocation(static_cast<GLuint>(m_program_id), name.c_str()),
      value);
}
void poggles::program::set_float(const std::string& name, float value) const
{
  glUniform1f(
      glGetUniformLocation(static_cast<GLuint>(m_program_id), name.c_str()),
      value);
}
