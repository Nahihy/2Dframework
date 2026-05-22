project(2Dframework)

set(SHADER_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/shaders"  CACHE STRING "" FORCE)
set(SHADER_EXTENSIONS  "glsl" CACHE STRING "" FORCE)

add_subdirectory(baseLibs/randerer)
add_subdirectory(baseLibs/database)

add_library(2Dframework STATIC
  src/background.c
  src/gameObject.c
  src/ground.c
  src/entity.c
  src/player.c
  src/ground.c
  src/world.c
)

target_include_directories(2Dframework PUBLIC include)
target_link_libraries(2Dframework PUBLIC randerer)
