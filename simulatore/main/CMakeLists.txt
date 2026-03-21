file(GLOB MAIN_SRC src/*.cpp)

add_executable(simulatore ${MAIN_SRC})

# OpenMP
find_package(OpenMP REQUIRED)
target_link_libraries(simulatore PRIVATE OpenMP::OpenMP_CXX)

# Collego le librerie create dagli altri moduli
target_link_libraries(simulatore PRIVATE Classi Utils Database Simulator Definitions)
