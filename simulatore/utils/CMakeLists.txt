file(GLOB UTILS_SRC src/*.cpp)

add_library(Utils ${UTILS_SRC})

target_include_directories(Utils PUBLIC include ../services/include)

find_package(SQLite3 REQUIRED)

# Collego la libreria Classi a Utils
target_link_libraries(Utils PUBLIC Classi)

# Collego la libreria servizi (TradingService) e Database a Utils
target_link_libraries(Utils PUBLIC services Database)

target_link_libraries(Utils PUBLIC SQLite::SQLite3)