cmake_minimum_required(VERSION 3.14)
project(2DframeworkShowcase)

find_package(OpenGL REQUIRED)

add_executable(2DframeworkShowcase libTest.c)


set(EMBED_SHADERS  ON  CACHE BOOL "" FORCE)
set(TEXTURE_DIR "textures" CACHE STRING "" FORCE)
set(TEXTURE_EXTENSIONS "png;jpg"   CACHE STRING "" FORCE)

add_subdirectory(2Dframework)
if(NOT WIN32)
  target_link_libraries(2DframeworkShowcase PRIVATE 2Dframework randerer glad glfw OpenGL::GL stb m)
else()
  target_link_libraries(2DframeworkShowcase PRIVATE 2Dframework randerer glad glfw OpenGL::GL stb)
endif()
