cmake_minimum_required(VERSION 3.14)

project(experimentationExamples C CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)


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
  target_link_libraries("${NAME}" PRIVATE raylib raylib_cpp)
  #target_link_libraries("${NAME}" PRIVATE drogon)
  
  # target_link_libraries("${NAME}" PRIVATE perlin_noise)
  target_compile_features("${NAME}" PRIVATE cxx_std_20)
  add_custom_target("run_${NAME}" COMMAND "${NAME}" VERBATIM)
  add_dependencies("run_${NAME}" "${NAME}")
  add_dependencies(run-examples "run_${NAME}")
endfunction()

# filesytem
add_example_cpp(list_all_files standard/filesystem/list_all_files)
add_example_cpp(thanos standard/filesystem/thanos)
add_example_cpp(recurvice standard/filesystem/recurvice)

add_example_cpp(files_on_demand files_on_demand)

add_example_cpp(call_function call_function)

# sort
add_example_cpp(sort standard/sort)

# Other
add_example_cpp(password_generator password_generator)

# json
add_example_cpp(json_dump json_dump)
add_example_cpp(json_letter json_letter)

# Raylib
add_example_cpp(rect raylib/world_of_cube/rect)
add_example_cpp(cube raylib/world_of_cube/cube)
add_example_cpp(perlin_noise raylib/world_of_cube/perlin_noise)
add_example_cpp(bench raylib/world_of_cube/bench)
add_example_cpp(block raylib/world_of_cube/block)
add_example_cpp(model raylib/world_of_cube/model)

# OpenCV
add_example_cpp(pixel_counter pixel_counter)
add_example_cpp(opencv_online opencv_online)

# Minecraft
# add_example_cpp(mc_readchunk mc_readchunk)

# Drogon
#add_example_cpp(helloworld_drogon drogon/helloworld_drogon)
#add_example_cpp(base_drogon drogon/base_drogon)

# Math