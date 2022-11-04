if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/experimentation-${PROJECT_VERSION}"
      CACHE PATH ""
  )
endif()

# Project is configured with no languages, so tell GNUInstallDirs the lib dir
set(CMAKE_INSTALL_LIBDIR lib CACHE PATH "")

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package experimentation)

install(
    DIRECTORY include/
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT experimentation_Development
)

install(
    TARGETS experimentation_experimentation
    EXPORT experimentationTargets
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT
)

# Allow package maintainers to freely override the path for the configs
set(
    experimentation_INSTALL_CMAKEDIR "${CMAKE_INSTALL_DATADIR}/${package}"
    CACHE PATH "CMake package config location relative to the install prefix"
)
mark_as_advanced(experimentation_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${experimentation_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT experimentation_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${experimentation_INSTALL_CMAKEDIR}"
    COMPONENT experimentation_Development
)

install(
    EXPORT experimentationTargets
    NAMESPACE experimentation::
    DESTINATION "${experimentation_INSTALL_CMAKEDIR}"
    COMPONENT experimentation_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
