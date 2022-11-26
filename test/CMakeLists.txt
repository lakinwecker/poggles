cmake_minimum_required(VERSION 3.14)

project(pogglesTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)
include(../cmake/windows-set-path.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(poggles REQUIRED)
  enable_testing()
endif()

find_package(glad REQUIRED)
find_package(doctest REQUIRED)

# ---- Tests ----

add_executable(poggles_test source/poggles_test.cpp)
target_link_libraries(
    poggles_test PRIVATE
    poggles::poggles
    glad::glad
    doctest::doctest
)
target_compile_features(poggles_test PRIVATE cxx_std_20)

add_test(NAME poggles_test COMMAND poggles_test)
windows_set_path(poggles_test poggles::poggles)

# ---- End-of-file commands ----

add_folders(Test)
