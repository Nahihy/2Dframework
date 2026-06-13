project(database)

add_library(database STATIC 
  src/database.c
  src/binary.c
  src/text.c
)
target_include_directories(database PUBLIC include)
if(NOT WIN32)
  target_link_libraries(database PRIVATE m)
endif()
