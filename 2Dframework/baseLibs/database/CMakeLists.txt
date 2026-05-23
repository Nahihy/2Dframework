project(database)

add_library(database STATIC src/database.c)
target_include_directories(database PUBLIC include)
target_link_libraries(database PRIVATE m)
