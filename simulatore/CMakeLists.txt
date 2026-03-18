cmake_minimum_required(VERSION 3.10)
project(Simulatore)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Aggiungo le directory con header
include_directories(
    Classi/include
    utils/include
)

# Aggiungo i moduli
add_subdirectory(Classi)
add_subdirectory(utils)
add_subdirectory(main)
