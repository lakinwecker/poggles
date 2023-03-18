#pragma once

#include <array>
#include <filesystem>
#include <span>
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
  program(
      std::initializer_list<std::pair<GLenum, std::string>> const& shaderFiles);
  program(std::filesystem::path const& vertex_path,
          std::filesystem::path const& fragment_path);

  // Public interface
  auto recompile() -> bool;
  auto use() const -> void;

  auto attach(shader_id id) -> void;

  explicit operator program_id() const
  {
    return static_cast<program_id>(m_program_handle);
  }

  auto set_bool(const std::string& name, bool value) const -> void;
  auto set_int(const std::string& name, int value) const -> void;
  auto set_texture_sampler(const std::string& name, GLint texture_num) const
      -> void;
  auto set_uint(const std::string& name, unsigned value) const -> void;
  auto set_float(const std::string& name, float value) const -> void;

  void set_vec2(const std::string& name, std::span<const float, 2> value) const;
  void set_vec3(const std::string& name, std::span<const float, 3> value) const;
  void set_vec4(const std::string& name, std::span<const float, 4> value) const;
  void set_mat4(const std::string& name,
                std::span<const float, 16> value) const;  // NOLINT

  auto set_double(const std::string& name, double value) const -> void;
  auto set_dvec3(const std::string& name,
                 std::span<const double, 3> value) const -> void;
  auto set_dmat4(const std::string& name,
                 std::span<const double, 16> value) const -> void;

  void set_uvec2(const std::string& name, std::span<const std::uint32_t, 2> value) const;
  void set_uvec3(const std::string& name, std::span<const std::uint32_t, 3> value) const;
  void set_uvec4(const std::string& name, std::span<const std::uint32_t, 4> value) const;

  template<size_t N>
  void set_float_array(const std::string& name,
                       int count,
                       std::array<float, N> array) const
  {
    glUniform1fv(glGetUniformLocation(static_cast<program_id>(m_program_handle),
                                      name.data()),
                 count,
                 array.data());
  }

private:
  program_handle m_program_handle;

  std::filesystem::path m_vertex_path;
  std::filesystem::path m_fragment_path;
};

auto checkLinkSuccess(program_id identifier) -> bool;

auto compileProgram(
    program_id program,
    std::initializer_list<std::pair<GLenum, std::string>> const& shaderFiles)
    -> bool;

}  // namespace poggles
