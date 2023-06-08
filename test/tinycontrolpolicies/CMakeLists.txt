cmake_minimum_required(VERSION 3.14)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

add_executable(
  TCP_tests
  test_pid.cc
  test_lti.cc
)
target_link_libraries(
  TCP_tests
  gtest_main
  tinycontrolpolicies
)

include(GoogleTest)
gtest_discover_tests(TCP_tests)
