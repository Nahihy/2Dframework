#pragma once

#include <cglm/cglm.h>

typedef struct {
  unsigned int ID;
} Shader;

unsigned int createShaderProgram(const char* vertexCode, const char* fragmentCode);
Shader createShader(const char* vertexPath, const char* fragmentPath);
void shaderUse(Shader* shader);
void shaderSetBool(Shader* shader, const char* name, int value);
void shaderSetInt(Shader* shader, const char* name, int value);
void shaderSetFloat(Shader* shader, const char* name, float value);
void shaderSetVec3(Shader* shader, const char* name, vec3 value);
void shaderSetMat4(Shader* shader, const char* name, mat4 value);
void shaderSetVec2(Shader* shader, const char* name, vec2 value);
void shaderDelete(Shader* shader);
