if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include"
      CACHE PATH ""
  )
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package poggles)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT poggles_Development
)

install(
    TARGETS poggles_poggles
    EXPORT pogglesTargets
    RUNTIME #
    COMPONENT poggles_Runtime
    LIBRARY #
    COMPONENT poggles_Runtime
    NAMELINK_COMPONENT poggles_Development
    ARCHIVE #
    COMPONENT poggles_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    poggles_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE PATH "CMake package config location relative to the install prefix"
)
mark_as_advanced(poggles_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${poggles_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT poggles_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${poggles_INSTALL_CMAKEDIR}"
    COMPONENT poggles_Development
)

install(
    EXPORT pogglesTargets
    NAMESPACE poggles::
    DESTINATION "${poggles_INSTALL_CMAKEDIR}"
    COMPONENT poggles_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
