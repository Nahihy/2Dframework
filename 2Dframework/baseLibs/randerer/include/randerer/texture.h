#pragma once
#include <glad/glad.h>

typedef struct {
  int unit;
  unsigned int ID;
} Texture;


Texture createTexture(GLenum unit);

void textureLoad(Texture* texture, const char* image, GLenum wrap, GLenum minFilter, GLenum maxFilter, GLenum colorType);
void textureUse(Texture* texture);
void textureDelete(Texture* texture);
