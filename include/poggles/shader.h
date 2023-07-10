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

struct shader_desc
{
  GLenum type;
  std::string name;
  std::string source;
};

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

auto shaderDescFromFile(GLenum type, std::filesystem::path filename)
    -> shader_desc;

auto compileShader(shader_id shader,
                   shader_desc shaderDesc,
                   // defines provided in the form "NAME [optional value]"
                   std::vector<std::string> const& defines = {}) -> bool;

auto addDefinesToShaderSource(std::string source,
                              std::vector<std::string> const& defines)
    -> std::string;

}  // namespace poggles
