# Tutti i file sorgente delle classi
file(GLOB CLASSI_SRC src/*.cpp)

add_library(services ${CLASSI_SRC})

target_include_directories(services PUBLIC include)
