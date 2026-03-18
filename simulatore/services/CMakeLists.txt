# Tutti i file sorgente delle classi
file(GLOB CLASSI_SRC src/*.cpp)

add_library(services ${CLASSI_SRC})

target_include_directories(services PUBLIC include)

# Aggiungo le include directories per le dipendenze
target_include_directories(services PUBLIC ../utils/include)
target_include_directories(services PUBLIC ../Classi/include)
target_include_directories(services PUBLIC ../db/include)

# Collego le dipendenze necessarie
target_link_libraries(services PUBLIC Utils Classi Database)
