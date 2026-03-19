cmake_minimum_required(VERSION 3.10)

# Define the definitions library (header-only)
add_library(Definitions INTERFACE)

# Set include directories
target_include_directories(Definitions
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Optionally export the library
target_include_directories(Definitions
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
