#include <regex>

#include "poggles/debug.h"

void poggles::gl_debug_output(GLenum source,
                              GLenum type,
                              GLuint identifier,
                              GLenum severity,
                              GLsizei,
                              const GLchar* message,
                              const void*)
{ // NOLINT
  // ignore non-significant error/warning codes
  // if(identifier == 131169 || identifier == 131185 || identifier == 131218 ||
  // identifier == 131204) return;
  if (identifier == 131204)
    return;

  std::string sourceStr;
  std::string typeStr;
  std::string severityStr;

  switch (source) {
    case GL_DEBUG_SOURCE_API:
      sourceStr = "API";
      break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
      sourceStr = "Window System";
      break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
      sourceStr = "Shader Compiler";
      break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
      sourceStr = "Third Party";
      break;
    case GL_DEBUG_SOURCE_APPLICATION:
      sourceStr = "Application";
      break;
    case GL_DEBUG_SOURCE_OTHER:
      sourceStr = "Other";
      break;
  }

  switch (type) {
    case GL_DEBUG_TYPE_ERROR:
      typeStr = "Error";
      break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      typeStr = "Deprecated Behaviour";
      break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      typeStr = "Undefined Behaviour";
      break;
    case GL_DEBUG_TYPE_PORTABILITY:
      typeStr = "Portability";
      break;
    case GL_DEBUG_TYPE_PERFORMANCE:
      typeStr = "Performance";
      break;
    case GL_DEBUG_TYPE_MARKER:
      typeStr = "Marker";
      break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
      typeStr = "Push Group";
      break;
    case GL_DEBUG_TYPE_POP_GROUP:
      typeStr = "Pop Group";
      break;
    case GL_DEBUG_TYPE_OTHER:
      typeStr = "Other";
      break;
  }

  std::string format = "[OPENGL] [{}] {} #{} -- {}: {}";
  std::string message_str = message;
  message_str =
      std::regex_replace(message_str, std::regex("^\\s+|\\s+$"), "$1");
  // TODO: Addin a way to configure where this debugging output goes.
  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
      severityStr = "high";
      // spdlog::critical(
      // format.c_str(), sourceStr, severityStr, identifier, typeStr,
      // message_str);
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      severityStr = "medium";
      // spdlog::warn(
      // format.c_str(), sourceStr, severityStr, identifier, typeStr,
      // message_str);
      break;
    case GL_DEBUG_SEVERITY_LOW:
      severityStr = "low";
      // spdlog::info(
      // format.c_str(), sourceStr, severityStr, identifier, typeStr,
      // message_str);
      break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      severityStr = "notification";
      // spdlog::debug(
      // format.c_str(), sourceStr, severityStr, identifier, typeStr,
      // message_str);
      break;
    default:
      break;
  }
}
