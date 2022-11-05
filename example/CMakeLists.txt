cmake_minimum_required(VERSION 3.14)

project(experimentationExamples CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

include(../cmake/lib/json.cmake)

if(PROJECT_IS_TOP_LEVEL)
  find_package(experimentation REQUIRED)
endif()

add_custom_target(run-examples)

function(add_example NAME)
  add_executable("${NAME}" "${NAME}.cpp")
  target_link_libraries("${NAME}" PRIVATE experimentation::experimentation)
  target_link_libraries("${NAME}" PRIVATE nlohmann_json::nlohmann_json)
  target_compile_features("${NAME}" PRIVATE cxx_std_20)
  add_custom_target("run_${NAME}" COMMAND "${NAME}" VERBATIM)
  add_dependencies("run_${NAME}" "${NAME}")
  add_dependencies(run-examples "run_${NAME}")
endfunction()

# filesytem
add_example(list_all_files)
add_example(call_function_from_string)

# json
add_example(json_dump)
add_example(json_letter)

add_folders(Example)
