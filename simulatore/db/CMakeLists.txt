# Build a small library for database access
file(GLOB DB_SRC src/*.cpp)

add_library(Database ${DB_SRC})

target_include_directories(Database PUBLIC include)

# SQLite is required for the database implementation
find_package(SQLite3 REQUIRED)
target_link_libraries(Database PUBLIC SQLite::SQLite3)
