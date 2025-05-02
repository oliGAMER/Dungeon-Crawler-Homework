cmake_minimum_required(VERSION 3.10)
project(SFML_Valgrind_Example)

set(CMAKE_CXX_STANDARD 17)

# Include your folders
include_directories(headers)
file(GLOB SRC_FILES "src/*.cpp")
file(GLOB MAIN_FILES "main/*.cpp")

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Build
add_executable(SFML_Valgrind ${SRC_FILES} ${MAIN_FILES})
target_link_libraries(SFML_Valgrind sfml-graphics sfml-window sfml-system)
