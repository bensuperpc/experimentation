cmake_minimum_required(VERSION 3.14)

project(experimentationTests LANGUAGES C CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)
include(../cmake/windows-set-path.cmake)

function(add_test_fn TEST_NAME)
  add_executable("${TEST_NAME}" "test/${TEST_NAME}.cpp")
  target_link_libraries("${TEST_NAME}" PRIVATE experimentation::experimentation gtest_main)
  target_link_libraries("${TEST_NAME}" PRIVATE nlohmann_json::nlohmann_json)

  if (OpenMP_FOUND OR OpenMP_CXX_FOUND)
    target_link_libraries("${TEST_NAME}" PRIVATE OpenMP::OpenMP_CXX)
  endif()

  if (CUDAToolkit_FOUND)
    target_include_directories("${TEST_NAME}" PRIVATE ${CUDAToolkit_INCLUDE_DIRS})
    target_link_libraries("${TEST_NAME}" PRIVATE CUDA::cudart)
  endif()

  add_test(NAME "${TEST_NAME}" COMMAND $<TARGET_FILE:${TEST_NAME}>)
  #target_compile_features("${TEST_NAME}" PRIVATE cxx_std_20)
endfunction()

function(add_bench_fn BENCH_NAME)
  add_executable("${BENCH_NAME}" "bench/${BENCH_NAME}.cpp")
  target_link_libraries("${BENCH_NAME}" PRIVATE experimentation::experimentation benchmark::benchmark_main)
  target_link_libraries("${BENCH_NAME}" PRIVATE nlohmann_json::nlohmann_json)

  if (OpenMP_FOUND OR OpenMP_CXX_FOUND)
    target_link_libraries("${BENCH_NAME}" PRIVATE OpenMP::OpenMP_CXX)
  endif()

  if (CUDAToolkit_FOUND)
    target_include_directories("${BENCH_NAME}" PRIVATE ${CUDAToolkit_INCLUDE_DIRS})
    target_link_libraries("${BENCH_NAME}" PRIVATE CUDA::cudart)
  endif()

  if(NOT CMAKE_BUILD_TYPE MATCHES Debug AND NOT CMAKE_BUILD_TYPE MATCHES Coverage)
    add_test(NAME "${BENCH_NAME}" COMMAND $<TARGET_FILE:${BENCH_NAME}>)
  elseif()
    message(STATUS "Disable ${BENCH_NAME}, Performance benchmark test only run on Release/RelWithDebInfo/MinSizeRel")
  endif()
  #target_compile_features("${TEST_NAME}" PRIVATE cxx_std_20)
endfunction()


if(NOT WIN32)
  include(../cmake/lib/gtest.cmake)
  include(../cmake/lib/benchmark.cmake)
  # Add tests
  #add_test_fn(math)

  # Add bench
  #add_bench_fn(map)
  #add_bench_fn(json_item)
  add_bench_fn(randomize)
  #add_bench_fn(vector_matrix)
  #add_bench_fn(vector_op)
  #add_bench_fn(math)
  #add_bench_fn(string_palindrome)
  #add_bench_fn(experimentation)
  #add_bench_fn(call_member)
endif()

# ---- End-of-file commands ----

add_folders(Test)
