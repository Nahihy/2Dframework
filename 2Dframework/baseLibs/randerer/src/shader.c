#include <randerer/shader.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#ifdef EMBED_SHADERS
#include "shader_lookup.h"
#endif

static char* readFile(const char* path) {
  FILE* file = fopen(path, "rb");
  if (!file) {
    perror("fopen error");
    printf("ERROR::SHADER::FILE_NOT_FOUND: %s\n", path);
    return NULL;
  }
  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);
  char* buffer = malloc(length + 1);
  if (fread(buffer, 1, length, file) != (size_t)length) {
    printf("ERROR::SHADER::FILE_READ_FAILED: %s\n", path);
    free(buffer);
    fclose(file);
    return NULL;
  }
  buffer[length] = '\0';
  fclose(file);
  return buffer;
}

static const char* loadShader(const char* path, int* needsFree) {
  *needsFree = 0;
#ifdef EMBED_SHADERS
  const char* src = lookupShader(path);
  if (!src) printf("ERROR::SHADER::NOT_FOUND_IN_EMBEDDED: %s\n", path);
  return src;
#else
  *needsFree = 1;
  return readFile(path);
#endif
}

Shader createShader(const char* vertexPath, const char* fragmentPath) {
  Shader shader;
  int vNeedsFree, fNeedsFree;
#ifndef EMBED_SHADERS
  char* relVertexPath = calloc(strlen(SHADER_DIR) + strlen("/") + strlen(vertexPath) + 1, sizeof(char));
  sprintf(relVertexPath, "%s/%s", SHADER_DIR, vertexPath);
  char* relFragmentPath = calloc(strlen(SHADER_DIR) + strlen("/") + strlen(fragmentPath) + 1, sizeof(char));
  sprintf(relFragmentPath, "%s/%s", SHADER_DIR, fragmentPath);
  const char* vertexCode   = loadShader(relVertexPath,   &vNeedsFree);
  const char* fragmentCode = loadShader(relFragmentPath, &fNeedsFree);
#else
  const char* vertexCode   = loadShader(vertexPath,   &vNeedsFree);
  const char* fragmentCode = loadShader(fragmentPath, &fNeedsFree);
#endif

  shader.ID = createShaderProgram(vertexCode, fragmentCode);

  if (vNeedsFree) free((char*)vertexCode);
  if (fNeedsFree) free((char*)fragmentCode);
#ifndef EMBED_SHADERS
  free(relVertexPath);
  free(relFragmentPath);
#endif
  return shader;
}

unsigned int createShaderProgram(const char* vertexCode, const char* fragmentCode) {
  unsigned int program; 
  int success;
  char infoLog[512];

  unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertexCode, NULL);
  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
  }

  unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragmentCode, NULL);
  glCompileShader(fragment);
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
  }

  program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);

  return program;
}

void shaderUse(Shader* shader) {
  glUseProgram(shader->ID);
}

void shaderSetBool(Shader* shader, const char* name, int value) {
  shaderUse(shader);
  glUniform1i(glGetUniformLocation(shader->ID, name), value);
}

void shaderSetInt(Shader* shader, const char* name, int value) {
  shaderUse(shader);
  glUniform1i(glGetUniformLocation(shader->ID, name), value);
}

void shaderSetFloat(Shader* shader, const char* name, float value) {
  shaderUse(shader);
  glUniform1f(glGetUniformLocation(shader->ID, name), value);
}

void shaderSetVec3(Shader* shader, const char* name, vec3 value) {
  shaderUse(shader);
  glUniform3f(glGetUniformLocation(shader->ID, name), value[0], value[1], value[2]);
}

void shaderSetMat4(Shader* shader, const char* name, mat4 value) {
  shaderUse(shader);
  glUniformMatrix4fv(glGetUniformLocation(shader->ID, name), 1, GL_FALSE, (float*)value);
}

void shaderSetVec2(Shader* shader, const char* name, vec2 value) {
  shaderUse(shader);
  glUniform2f(glGetUniformLocation(shader->ID, name), value[0], value[1]);
}

void shaderDelete(Shader* shader) {
  glDeleteProgram(shader->ID);
}



