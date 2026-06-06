cmake_minimum_required(VERSION 3.10)

# Generate compile_commands.json for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Hoshizora)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler-specific optimization flags
add_compile_options(-march=native -O3 -ffast-math -funroll-loops)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glm)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/stb)

# Source files
file(GLOB_RECURSE SOURCES src/*.cpp)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})
