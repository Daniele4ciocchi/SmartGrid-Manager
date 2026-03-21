file(GLOB SIMULATOR_SRC
    src/*.cpp
)

add_library(Simulator ${SIMULATOR_SRC})

target_include_directories(Simulator PUBLIC include)

find_package(OpenMP REQUIRED)

target_link_libraries(Simulator PUBLIC Definitions Classi Utils Database OpenMP::OpenMP_CXX)