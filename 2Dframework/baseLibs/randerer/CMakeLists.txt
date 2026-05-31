cmake_minimum_required(VERSION 3.14)


if(NOT DEFINED EMBED_SHADERS)
  option(EMBED_SHADERS "Embed shaders into binary at compile time" OFF)
endif()
if(NOT DEFINED SHADER_DIR)
  set(SHADER_DIR "shaders" CACHE STRING "Shader directory")
endif()
if(NOT DEFINED SHADER_EXTENSIONS)
  set(SHADER_EXTENSIONS "glsl" CACHE STRING "Shader file extensions")
endif()

if(NOT DEFINED TEXTURE_DIR)
  set(TEXTURE_DIR "textures" CACHE STRING "Texture directory")
endif()

add_library(randerer STATIC
  src/camera.c
  src/shader.c
  src/window.c
  src/mesh.c
  src/randerer.c
  src/texture.c
)

find_package(OpenGL REQUIRED)

set(CGLM_SHARED OFF CACHE BOOL "" FORCE)
set(CGLM_STATIC ON CACHE BOOL "" FORCE)

add_subdirectory(baseLibs/cglm)
add_subdirectory(baseLibs/glad)
add_subdirectory(baseLibs/stb)

if(NOT TARGET glfw)
  add_subdirectory(baseLibs/glfw)
endif()
target_include_directories(randerer PUBLIC include)
target_link_libraries(randerer PUBLIC glfw glad stb OpenGL::GL cglm m)

target_compile_definitions(randerer PUBLIC
  SHADER_DIR="shaders"
  TEXTURE_DIR="textures"
)
if(IS_ABSOLUTE "${SHADER_DIR}")
  set(SHADER_SEARCH_DIR "${SHADER_DIR}")
else()
  set(SHADER_SEARCH_DIR "${CMAKE_SOURCE_DIR}/${SHADER_DIR}")
endif()

if(EMBED_SHADERS)

  set(GLSL_FILES "")
  foreach(EXT ${SHADER_EXTENSIONS})
    file(GLOB_RECURSE TMP "${SHADER_SEARCH_DIR}/*.${EXT}")
    list(APPEND GLSL_FILES ${TMP})
  endforeach()
  list(GET SHADER_EXTENSIONS 0 FIRST_SHADER_EXT)
  set(GENERATED_SHADER_HEADERS "")
  message(STATUS "glsl files = " ${GLSL_FILES})
  foreach(GLSL_FILE ${GLSL_FILES})
    file(RELATIVE_PATH REL_PATH "${SHADER_SEARCH_DIR}" "${GLSL_FILE}")
    string(REPLACE ".${FIRST_SHADER_EXT}" ".h" OUTPUT_REL "${REL_PATH}")
    foreach(EXT ${SHADER_EXTENSIONS})
      string(REPLACE ".${EXT}" ".h" OUTPUT_REL "${OUTPUT_REL}")
    endforeach()
    set(OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/embedded_shaders/${OUTPUT_REL}")

    string(REPLACE "/" "_" VAR_NAME "${REL_PATH}")
    string(REPLACE "." "_" VAR_NAME "${VAR_NAME}")
    string(REPLACE "-" "_" VAR_NAME "${VAR_NAME}")
    string(TOUPPER "${VAR_NAME}" GUARD)
    string(PREPEND GUARD "SHADER_")

    add_custom_command(
      OUTPUT "${OUTPUT_FILE}"
      COMMAND ${CMAKE_COMMAND}
        -DINPUT_FILE=${GLSL_FILE}
        -DOUTPUT_FILE=${OUTPUT_FILE}
        -DVAR_NAME=${VAR_NAME}
        -DGUARD=${GUARD}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/embed_shader.cmake
      DEPENDS "${GLSL_FILE}"
      COMMENT "Embedding shader ${REL_PATH}"
    )
    list(APPEND GENERATED_SHADER_HEADERS "${OUTPUT_FILE}")
  endforeach()

  set(MASTER_SHADER_HEADER "${CMAKE_CURRENT_BINARY_DIR}/embedded_shaders/all_shaders.h")
  set(MASTER_SHADER_CONTENT "#ifndef SHADER_ALL_SHADERS_H\n#define SHADER_ALL_SHADERS_H\n")
  set(SHADER_LOOKUP_HEADER "${CMAKE_CURRENT_BINARY_DIR}/embedded_shaders/shader_lookup.h")
  set(SHADER_LOOKUP_CONTENT "#ifndef SHADER_LOOKUP_H\n#define SHADER_LOOKUP_H\n")
  string(APPEND SHADER_LOOKUP_CONTENT "#include \"all_shaders.h\"\n#include <string.h>\n\n")
  string(APPEND SHADER_LOOKUP_CONTENT "static inline const char* lookupShader(const char* path) {\n")

  foreach(GLSL_FILE ${GLSL_FILES})
    file(RELATIVE_PATH REL_PATH "${SHADER_SEARCH_DIR}" "${GLSL_FILE}")
    string(REPLACE ".${FIRST_SHADER_EXT}" ".h" OUTPUT_REL "${REL_PATH}")
    foreach(EXT ${SHADER_EXTENSIONS})
      string(REPLACE ".${EXT}" ".h" OUTPUT_REL "${OUTPUT_REL}")
    endforeach()
    string(APPEND MASTER_SHADER_CONTENT "#include \"${OUTPUT_REL}\"\n")
    string(REPLACE "/" "_" VAR_NAME "${REL_PATH}")
    string(REPLACE "." "_" VAR_NAME "${VAR_NAME}")
    string(REPLACE "-" "_" VAR_NAME "${VAR_NAME}")
    string(APPEND SHADER_LOOKUP_CONTENT "  if (strcmp(path, \"${REL_PATH}\") == 0) return ${VAR_NAME};\n")
  endforeach()

  string(APPEND MASTER_SHADER_CONTENT "#endif\n")
  string(APPEND SHADER_LOOKUP_CONTENT "  return 0;\n}\n#endif\n")
  file(WRITE "${MASTER_SHADER_HEADER}" "${MASTER_SHADER_CONTENT}")
  file(WRITE "${SHADER_LOOKUP_HEADER}" "${SHADER_LOOKUP_CONTENT}")

  add_custom_target(randerer_shaders DEPENDS ${GENERATED_SHADER_HEADERS})
  add_dependencies(randerer randerer_shaders)
  target_include_directories(randerer PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/embedded_shaders)
  target_compile_definitions(randerer PRIVATE EMBED_SHADERS)
else()
  add_custom_target(copy-shader-dir ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADER_SEARCH_DIR} ${CMAKE_BINARY_DIR}/shaders)
endif()

if(IS_ABSOLUTE "${TEXTURE_DIR}")
  set(TEXTURE_SEARCH_DIR "${TEXTURE_DIR}")
else()
  set(TEXTURE_SEARCH_DIR "${CMAKE_SOURCE_DIR}/${TEXTURE_DIR}")
endif()

add_custom_target(copy-texture-dir ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${TEXTURE_SEARCH_DIR} ${CMAKE_BINARY_DIR}/textures)
