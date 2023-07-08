# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.

cmake_minimum_required (VERSION 3.12)


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

project ("tinymatrixmath" C CXX)
set (CMAKE_CXX_STANDARD 20) # We use new features like std::endian, std::float_32, ...

# Options
option(${PROJECT_NAME}_BUILD_TESTS    "Build all projects in the 'test' folder (requires GoogleTest)"     ON)
option(${PROJECT_NAME}_BUILD_EXAMPLES "Build all projects in the 'examples' folder"                       ON)
option(${PROJECT_NAME}_BUILD_DOCS     "Build documentation (requires Doxygen)"                            ON)


# Set the macro/helper directory 
set(MACRO_DIRECTORY ${CMAKE_HOME_DIRECTORY}/CMake/macros)
message("${BoldMagenta}CMake utility directory: ${MACRO_DIRECTORY}${ColorReset}")


# By using CMake, it's safe to assume that the Standard Library is available.
# (That's not the case when we're using this library in an Arduino library!)
# Manually define a compile-time definition
# add_compile_definitions(USING_STANDARD_LIBRARY=1)

# TinyMatrixMath
add_library (${PROJECT_NAME}
src/TinyMatrixMath.hpp
src/TMM_enable_if.hpp
src/TMM_matrix.hpp
src/TMM_matrix.cpp
src/TinyMatrixMath.cpp
)
target_link_libraries (tinymatrixmath)
target_include_directories(tinymatrixmath PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_compile_definitions(tinymatrixmath PUBLIC USING_STANDARD_LIBRARY)

message("${BoldYellow}Library search complete!${ColorReset}")

  
# Include tests.
if(${PROJECT_NAME}_BUILD_TESTS)
include(CTest)
add_subdirectory ("test")
endif()


# Include examples.
if(${PROJECT_NAME}_BUILD_EXAMPLES)
add_subdirectory(cmake_examples)
endif()

# Documentation
if(${PROJECT_NAME}_BUILD_DOCS)
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
  ${PROJECT_NAME}_docs
  ${PROJECT_SOURCE_DIR}/README.md
  ${PROJECT_SOURCE_DIR}/src
  COMMENT "Generate docs"
)
endif()
endif()