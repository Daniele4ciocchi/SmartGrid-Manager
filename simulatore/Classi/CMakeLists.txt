file(GLOB SOURCES "src/*.cpp")

add_library(smartgrid_classes STATIC ${SOURCES})
target_include_directories(smartgrid_classes PUBLIC src)
