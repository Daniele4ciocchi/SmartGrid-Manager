cmake_minimum_required(VERSION 3.20)

project(SmartGridManager LANGUAGES CXX)

# Global C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default compile options (can be refined per-target)
if(MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Subprojects
add_subdirectory(simulatore)

# Optionally, you could add tests here later
# enable_testing()
