add_subdirectory(Classi)

add_executable(programma main/src/main.cpp)
target_link_libraries(programma smartgrid_classes)
target_include_directories(programma PRIVATE Classi/src)
