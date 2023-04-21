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
include(cmake/dependencies.cmake)
include(cmake/targets.cmake)

# ---- Dependencies ----
poggles_find_dependencies_thirdparty()

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
