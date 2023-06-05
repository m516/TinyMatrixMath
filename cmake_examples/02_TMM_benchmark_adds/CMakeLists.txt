# This is the name of the executable
set(EXECUTABLE_NAME TMM_02_Benchmark_Adds)

# Add source to this project's executable.
add_executable (${EXECUTABLE_NAME} "main.cpp")

# Add tests and install targets if needed.
TARGET_LINK_LIBRARIES (${EXECUTABLE_NAME} tinymatrixmath)