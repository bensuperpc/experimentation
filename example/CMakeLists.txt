cmake_minimum_required(VERSION 3.14)

project(experimentationExamples C CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

include(../cmake/lib/json.cmake)
include(../cmake/lib/opencv.cmake)
include(../cmake/lib/openmp.cmake)
include(../cmake/lib/raylib.cmake)
include(../cmake/lib/raylib-cpp.cmake)

if(PROJECT_IS_TOP_LEVEL)
  find_package(experimentation REQUIRED)
endif()

add_custom_target(run-examples)
function(add_example_cpp NAME PATH)
  add_executable("${NAME}" "${PATH}.cpp")
  target_link_libraries("${NAME}" PRIVATE experimentation::experimentation)
  target_link_libraries("${NAME}" PRIVATE nlohmann_json::nlohmann_json)
  target_link_libraries("${NAME}" PRIVATE OpenMP::OpenMP_CXX)
  target_link_libraries("${NAME}" PRIVATE raylib raylib_cpp)
  target_link_libraries("${NAME}" PRIVATE ${OpenCV_LIBS})
  target_compile_features("${NAME}" PRIVATE cxx_std_20)
  add_custom_target("run_${NAME}" COMMAND "${NAME}" VERBATIM)
  add_dependencies("run_${NAME}" "${NAME}")
  add_dependencies(run-examples "run_${NAME}")
endfunction()

# filesytem
add_example_cpp(list_all_files standard/list_all_files)

add_example_cpp(call_function_from_string call_function_from_string)

# sort
add_example_cpp(sort sort)

# json
add_example_cpp(json_dump json_dump)
add_example_cpp(json_letter json_letter)

# Raylib
add_example_cpp(rect raylib/rect)

# OpenCV
add_example_cpp(pixel_counter pixel_counter)