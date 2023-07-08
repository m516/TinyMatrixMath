cmake_minimum_required(VERSION 3.14)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

include(GoogleTest)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_executable(
  ${PROJECT_NAME}_tests
  inline_matrix_ops.cc
  matrix_generators.cc
  matrix_inverse.cc
  util_float_eq.cc
)
target_link_libraries(
  ${PROJECT_NAME}_tests
  ${PROJECT_NAME}
)

gtest_discover_tests(${PROJECT_NAME}_tests)
