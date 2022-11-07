#include <cstring>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <vector>

#include "poggles/shader.h"

poggles::shader::shader(std::filesystem::path path, GLenum type)
    : m_shader_id(type)
    , m_type(type)
    , m_path(std::move(path))
{
  if (!compile()) {
    throw poggles::shader_compile_exception("Shader did not compile");
  }
}

auto poggles::shader::compile() -> bool
{
  // read shader source
  std::string source_string;
  std::ifstream file;

  // ensure ifstream objects can throw exceptions:
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    // open file
    file.open(m_path.string());
    std::stringstream source_stream;

    // read file buffer contents into stream
    source_stream << file.rdbuf();

    // close file handler
    file.close();

    // convert stream into string
    source_string = source_stream.str();
  } catch (std::ifstream::failure const&) {
    std::string error_msg;
    // TODO: get this working generally
    //#ifdef DGGS_VISUALSTUDIO
    // error_msg.resize(94); //
    // https://developercommunity.visualstudio.com/t/strerrorlen-s-is-not-supported/160287
    // strerror_s(error_msg.data(), error_msg.size(), errno);
    //#else
    // error_msg = strerror(errno);
    //#endif

    // spdlog::error("[SHADER] reading {}:\n{}", m_path.string(), errorMsg);
    return false;
  }

  GLchar const* source_code = source_string.c_str();

  // compile shader
  glShaderSource(static_cast<GLuint>(m_shader_id), 1, &source_code, nullptr);
  glCompileShader(static_cast<GLuint>(m_shader_id));

  // Always log the info, just change the severity
  // based on whether it's an error or not
  GLint log_length = -1;
  glGetShaderiv(
      static_cast<GLuint>(m_shader_id), GL_INFO_LOG_LENGTH, &log_length);
  std::string log;
  log.resize(static_cast<size_t>(log_length));
  glGetShaderInfoLog(
      static_cast<GLuint>(m_shader_id), log_length, nullptr, log.data());

  // check for errors
  GLint success = -1;
  glGetShaderiv(static_cast<GLuint>(m_shader_id), GL_COMPILE_STATUS, &success);
  // if (success == 0) {
  // spdlog::error("[SHADER] compilation log {}:\n{}", m_path.string(), log);
  //} else {
  // spdlog::debug("[SHADER] compilation log {}:\n{}", m_path.string(), log);
  //}
  return success != 0;
}
