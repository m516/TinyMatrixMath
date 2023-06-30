# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.

cmake_minimum_required (VERSION 3.12)

# Options
option(TINYMATRIXMATH_BUILD_TESTS    "Build all projects in the 'test' folder"     ON)
option(TINYMATRIXMATH_BUILD_EXAMPLES "Build all projects in the 'examples' folder" ON)
option(TINYMATRIXMATH_BUILD_DOCS     "Build all projects in the 'examples' folder" ON)


# Define colors
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColorReset "${Esc}[m")
  set(ColorBold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldGreen   "${Esc}[1;32m")
  set(BoldYellow  "${Esc}[1;33m")
  set(BoldBlue    "${Esc}[1;34m")
  set(BoldMagenta "${Esc}[1;35m")
  set(BoldCyan    "${Esc}[1;36m")
  set(BoldWhite   "${Esc}[1;37m")
endif()

project ("TinyMatrixMath" C CXX)
set (CMAKE_CXX_STANDARD 20) # We use new features like std::endian, std::float_32, ...


# Set the macro/helper directory 
set(MACRO_DIRECTORY ${CMAKE_HOME_DIRECTORY}/CMake/macros)
message("${BoldMagenta}CMake utility directory: ${MACRO_DIRECTORY}${ColorReset}")


# By using CMake, it's safe to assume that the Standard Library is available.
# (That's not the case when we're using this library in an Arduino library!)
# Manually define a compile-time definition
# add_compile_definitions(USING_STANDARD_LIBRARY=1)


# Serializer
add_library (serializer ${PROJECT_SOURCES}
src/Serializer.hpp
src/Serializer.cpp
)
target_link_libraries (serializer)
target_include_directories(serializer PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_compile_definitions(serializer PUBLIC USING_STANDARD_LIBRARY)


# TinyMatrixMath
add_library (tinymatrixmath ${PROJECT_SOURCES}
src/TinyMatrixMath.hpp
src/TMM_enable_if.hpp
src/TMM_matrix.hpp
src/TMM_matrix.cpp
src/TinyMatrixMath.cpp
)
target_link_libraries (tinymatrixmath)
target_include_directories(tinymatrixmath PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_compile_definitions(tinymatrixmath PUBLIC USING_STANDARD_LIBRARY)

# TinyControlPolicy
add_library (tinycontrolpolicies ${PROJECT_SOURCES}
src/TCP_control_policy.hpp 
src/TCP_controller_LTI.hpp 
src/TCP_controller_passthrough.hpp 
src/TCP_controller_PID.hpp 
src/TCP_robot_link.hpp 
src/TCP_states.hpp
src/TCP_utils.hpp
src/TCP_utils.cpp
src/TinyControlPolicies.cpp 
src/TinyControlPolicies.hpp
)
target_link_libraries (tinycontrolpolicies)
target_include_directories(tinycontrolpolicies PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_compile_definitions(tinycontrolpolicies PUBLIC USING_STANDARD_LIBRARY)



# Google Test
message("${BoldYellow}Using local copy of the Google Test framework${ColorReset}")
add_subdirectory(thirdparty/cmake/googletest)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)


message("${BoldYellow}Library search complete!${ColorReset}")

  
# Include tests.
if(TINYMATRIXMATH_BUILD_TESTS)
include(CTest)
add_subdirectory ("test")
endif()


# Include examples.
if(TINYMATRIXMATH_BUILD_EXAMPLES)
add_subdirectory(cmake_examples)
endif()

# Documentation
if(TINYMATRIXMATH_BUILD_DOCS)
find_package(Doxygen)
if(DOXYGEN_FOUND)
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_GENERATE_MAN  YES)
set(DOXYGEN_GENERATE_TREEVIEW      YES)
set(DOXYGEN_DISABLE_INDEX          NO)
set(DOXYGEN_FULL_SIDEBAR           NO)
set(DOXYGEN_HTML_STYLESHEET        ${PROJECT_SOURCE_DIR}/docs/thirdparty/doxygen-awesome-css/doxygen-awesome.css)
set(DOXYGEN_HTML_COLORSTYLE        LIGHT)
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${PROJECT_SOURCE_DIR}/README.md)
doxygen_add_docs(
  TMM_doxygen
  ${PROJECT_SOURCE_DIR}/README.md
  ${PROJECT_SOURCE_DIR}/src
  COMMENT "Generate docs"
)
endif()
endif()