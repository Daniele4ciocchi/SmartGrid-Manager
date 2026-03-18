file(GLOB UTILS_SRC src/*.cpp)

add_library(Utils ${UTILS_SRC})

target_include_directories(Utils PUBLIC include)

find_package(SQLite3 REQUIRED)

# Collego la libreria Classi a Utils
target_link_libraries(Utils PUBLIC Classi)

# Collego la libreria Database a Utils (per utilizzare Database.h e le sue implementazioni)
target_link_libraries(Utils PUBLIC Database)

target_link_libraries(Utils PUBLIC SQLite::SQLite3)