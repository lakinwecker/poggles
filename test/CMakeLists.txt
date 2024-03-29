cmake_minimum_required(VERSION 3.14)

project(pogglesTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)
include(../cmake/windows-set-path.cmake)
include(poggles_dependencies)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(poggles REQUIRED)
  enable_testing()
endif()


poggles_find_dependencies_thirdparty()
poggles_dependency_targets()

# ---- Tests ----
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_link_options(-Wl,--disable-new-dtags)
endif()

add_executable(poggles_test source/poggles_test.cpp)
target_link_libraries(
    poggles_test PRIVATE
    poggles::poggles
    ${POGGLES_DEPENDENCY_TARGETS}
)
target_compile_features(poggles_test PRIVATE cxx_std_20)

windows_set_path(poggles_test poggles::poggles)

# ---- End-of-file commands ----
add_folders(Test)
add_test(NAME poggles_test COMMAND poggles_test)
