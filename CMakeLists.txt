cmake_minimum_required(VERSION 3.14)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(cmake/prelude.cmake)

file(READ "version.txt" ver)
string(STRIP ${ver} ver)
project(
    poggles
    VERSION ${ver}
    DESCRIPTION "Poggles is for OpenGGL EnvironmentS"
    HOMEPAGE_URL "https://github.com/lakinwecker/poggles"
    LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)
include(poggles_dependencies)
include(poggles_targets)
include(givde_windows)

# ---- Dependencies ----
poggles_find_dependencies_thirdparty()
poggles_dependency_targets()
givde_attempt_at_sanity_with_windows()

# ---- Declare library ----
poggles_declare_library("")

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
