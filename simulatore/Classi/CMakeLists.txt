# Tutti i file sorgente delle classi
file(GLOB CLASSI_SRC src/*.cpp)

add_library(Classi ${CLASSI_SRC})

target_include_directories(Classi PUBLIC include)
