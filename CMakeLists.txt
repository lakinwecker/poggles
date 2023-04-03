cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    poggles
    VERSION 0.2.0
    DESCRIPTION "Poggles is for OpenGGL EnvironmentS"
    HOMEPAGE_URL "https://github.com/lakinwecker/poggles"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    poggles_poggles
    source/poggles/buffer.cpp
    source/poggles/debug.cpp
    source/poggles/program.cpp
    source/poggles/shader.cpp
    source/poggles/stb_image.cpp
    source/poggles/vertex_array.cpp
    source/poggles/texture.cpp
    source/poggles/gl_function.cpp
)
add_library(poggles::poggles ALIAS poggles_poggles)

include(GenerateExportHeader)
generate_export_header(
    poggles_poggles
    BASE_NAME poggles
    EXPORT_FILE_NAME export/poggles/poggles_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(poggles_poggles PUBLIC POGGLES_STATIC_DEFINE)
endif()

set_target_properties(
    poggles_poggles PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME poggles
    OUTPUT_NAME poggles
)

target_include_directories(
    poggles_poggles ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    poggles_poggles SYSTEM
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(poggles_poggles PUBLIC cxx_std_20)

find_package(glad REQUIRED)
find_package(stb REQUIRED)
find_package(opengl_system REQUIRED)
target_link_libraries(
    poggles_poggles PRIVATE
    glad::glad
    stb::stb
    opengl::opengl
)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT poggles_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of poggles"
  )
endif()

include(cmake/dev-mode.cmake)
