#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "poggles/handle.h"
#include "poggles/poggles_export.hpp"

namespace poggles
{

class POGGLES_EXPORT shader_compile_exception : public std::runtime_error
{
public:
  explicit shader_compile_exception(const char* message)
      : std::runtime_error(message)
  {
  }

  explicit shader_compile_exception(const std::string& message)
      : std::runtime_error(message)
  {
  }
};

class POGGLES_EXPORT shader
{
public:
  shader(std::filesystem::path path, GLenum type);

  // Public interface
  [[nodiscard]] auto get_path() const -> std::string { return m_path; }
  [[nodiscard]] auto get_type() const -> GLenum { return m_type; }
  explicit operator GLuint() const { return static_cast<GLuint>(m_shader_id); }

private:
  shader_id m_shader_id;
  GLenum m_type;

  std::filesystem::path m_path;

  auto compile() -> bool;
};

}  // namespace poggles
