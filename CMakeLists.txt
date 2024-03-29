cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    experimentation
    VERSION 0.1.0
    DESCRIPTION "experimentation"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES C CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

include(cmake/lib/json.cmake)
include(cmake/lib/opencv.cmake)
include(cmake/lib/openmp.cmake)
include(cmake/lib/raylib.cmake)
include(cmake/lib/raylib-cpp.cmake)
include(cmake/lib/perlin_noise.cmake)
#include(cmake/lib/drogon.cmake)

# -D_FORTIFY_SOURCE=3

# ---- Declare library ----

add_library(experimentation_experimentation INTERFACE)
add_library(experimentation::experimentation ALIAS experimentation_experimentation)

set_property(
    TARGET experimentation_experimentation PROPERTY
    EXPORT_NAME experimentation
)

target_include_directories(
    experimentation_experimentation ${warning_guard}
    INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_compile_features(experimentation_experimentation INTERFACE cxx_std_20)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
  #set(BUILD_EXAMPLES_DEFAULT ON CACHE BOOL "Build examples tree." FORCE)
  #if(BUILD_EXAMPLES)
  #  message(STATUS "Building examples")
  #  add_subdirectory(example)
  #else()
  #  message(STATUS "Not building examples")
  #endif()
  add_subdirectory(example)
endif()

# ---- Developer mode ----

if(NOT experimentation_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of experimentation"
  )
endif()

include(cmake/dev-mode.cmake)
