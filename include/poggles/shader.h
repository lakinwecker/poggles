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

auto compileShader(shader_id shader, std::filesystem::path path) -> bool;

}  // namespace poggles
