cmake_minimum_required(VERSION 3.14)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

add_executable(
  Serializer_tests
  test_serializer.cc
)
target_link_libraries(
  Serializer_tests
  gtest_main
  serializer
)

include(GoogleTest)
gtest_discover_tests(Serializer_tests)
