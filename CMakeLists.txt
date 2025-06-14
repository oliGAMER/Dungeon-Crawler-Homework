# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(HomeworkProject2)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SFML package
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Set output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
include_directories(headers)

# Gather all source files
file(GLOB SRC_FILES "src/*.cpp")
file(GLOB MAIN_FILES "main/*.cpp")

# Add the executable
add_executable(HomeworkProject2 ${SRC_FILES} ${MAIN_FILES})

# Link SFML libraries
target_link_libraries(HomeworkProject2 sfml-graphics sfml-window sfml-system)
