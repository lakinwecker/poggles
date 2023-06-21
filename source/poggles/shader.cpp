#include <cstring>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <vector>

#include "poggles/shader.h"

auto poggles::addDefinesToShaderSource(std::string source,
                                       std::vector<std::string> const& defines)
    -> std::string
{
  std::string defineString;
  for (auto define : defines) {
    defineString += std::string("#define ") + define + std::string("\n");
  }

  // The first line of non-comment non-whitespace code must be the version
  // number. We put the defines on the first line after this
  auto versionLoc = source.find("#version");
  auto newLineLoc = source.find('\n', versionLoc);

  if (newLineLoc != std::string::npos && newLineLoc + 1 < source.length()) {
    source.insert(newLineLoc + 1, defineString.c_str());
  }
  // else do nothing, the shader will fail to compile anyways

  return source;
}

auto poggles::compileShader(shader_id shader,
                            std::filesystem::path path,
                            std::vector<std::string> const& defines) -> bool
{
  // read shader source
  std::string source_string;
  std::ifstream file;

  // ensure ifstream objects can throw exceptions:
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    // open file
    file.open(path.string());
    std::stringstream source_stream;

    // read file buffer contents into stream
    source_stream << file.rdbuf();

    // close file handler
    file.close();

    // convert stream into string
    source_string = source_stream.str();
    source_string = poggles::addDefinesToShaderSource(source_string, defines);

    // Add defines to shader
  } catch (std::ifstream::failure const&) {
    std::string error_msg;
    // TODO: get this working generally
    // #ifdef DGGS_VISUALSTUDIO
    // error_msg.resize(94); //
    // https://developercommunity.visualstudio.com/t/strerrorlen-s-is-not-supported/160287
    // strerror_s(error_msg.data(), error_msg.size(), errno);
    // #else
    // #endif

    error_msg = strerror(errno);
    std::cerr << "[SHADER] reading " << path.string() << ":\n"
              << error_msg << std::endl;
    return false;
  }

  GLchar const* source_code = source_string.c_str();

  // compile shader
  glShaderSource(shader, 1, &source_code, nullptr);
  glCompileShader(shader);

  // Always log the info, just change the severity
  // based on whether it's an error or not
  GLint log_length = -1;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
  std::string log;
  log.resize(static_cast<size_t>(log_length));
  glGetShaderInfoLog(shader, log_length, nullptr, log.data());

  // check for errors
  GLint success = -1;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success == 0) {
    std::cerr << "[SHADER] compilation log " << path.string() << ":\n"
              << log << std::endl;
  }
  return success != 0;
}
