#pragma once

typedef struct {
  int unit;
  unsigned int ID;
} Texture;


Texture createTexture(int unit);

void textureLoad(Texture* texture, const char* image, int wrap, int minFilter, int maxFilter, int colorType);
void textureUse(Texture* texture);
void textureDelete(Texture* texture);
