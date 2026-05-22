cmake_minimum_required(VERSION 3.14)
project(test)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(test libTest.c)


set(EMBED_SHADERS  ON  CACHE BOOL "" FORCE)
set(EMBED_TEXTURES OFF CACHE BOOL "" FORCE)
set(TEXTURE_DIR "textures" CACHE STRING "" FORCE)
set(TEXTURE_EXTENSIONS "png;jpg"   CACHE STRING "" FORCE)

add_subdirectory(2Dframework)

target_link_libraries(test PRIVATE 2Dframework randerer glad glfw OpenGL::GL m stb)
