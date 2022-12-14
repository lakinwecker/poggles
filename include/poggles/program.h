#pragma once

#include <array>
#include <filesystem>
#include <string>

#include "poggles/handle.h"
#include "poggles/poggles_export.hpp"
#include "poggles/shader.h"

namespace poggles
{

class POGGLES_EXPORT shader_link_exception : public std::runtime_error
{
public:
  explicit shader_link_exception(const char* message)
      : std::runtime_error(message)
  {
  }

  explicit shader_link_exception(const std::string& message)
      : std::runtime_error(message)
  {
  }
};

class POGGLES_EXPORT program
{
public:
  program(std::filesystem::path const& vertex_path,
          std::filesystem::path const& fragment_path);

  // Public interface
  auto recompile() -> bool;
  auto use() const -> void;

  auto attach(GLuint shader_id) -> void
  {
    glAttachShader(static_cast<GLuint>(m_program_id), shader_id);
  }

  explicit operator GLuint() const { return static_cast<GLuint>(m_program_id); }

  auto set_bool(const std::string& name, bool value) const -> void;
  auto set_int(const std::string& name, int value) const -> void;
  auto set_texture_sampler(const std::string& name, GLint texture_num) const
      -> void;
  auto set_uint(const std::string& name, unsigned value) const -> void;
  auto set_float(const std::string& name, float value) const -> void;

  template<size_t N>
  void set_float_array(const std::string& name,
                       int count,
                       std::array<float, N> array) const
  {
    glUniform1fv(
        glGetUniformLocation(static_cast<GLuint>(m_program_id), name.data()),
        count,
        array.data());
  }

private:
  auto check_link_success(GLuint identifier) -> bool;

  program_id m_program_id;

  shader m_vertex;
  shader m_fragment;
};

}  // namespace poggles
