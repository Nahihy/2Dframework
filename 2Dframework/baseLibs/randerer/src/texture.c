#include <randerer/texture.h>
#include <glad/glad.h>
#include <stb_image.h>

#include <stdio.h>
#include <string.h>

#ifdef EMBED_TEXTURES
#include "texture_lookup.h"
#endif


static unsigned char* loadTextureData(const char* path, int* w, int* h, int* channels, int* needsFree) {
  *needsFree = 0;
#ifdef EMBED_TEXTURES
  TextureData td = lookupTexture(path);
  if (!td.data) {
    printf("ERROR::TEXTURE::NOT_FOUND_IN_EMBEDDED: %s\n", path);
    return NULL;
  }
  *needsFree = 1;
  return stbi_load_from_memory(td.data, td.len, w, h, channels, 0);
#else
  char* fullPath = malloc(strlen(TEXTURE_DIR) + strlen("/") + strlen(path) + 1);
  sprintf(fullPath, "%s/%s", TEXTURE_DIR, path);
  *needsFree = 1;
  unsigned char* data = stbi_load(fullPath, w, h, channels, 0);
  free(fullPath);
  return data;
#endif


}
Texture createTexture(int unit) {
  Texture texture;
  texture.unit = unit;
  glGenTextures(1, &texture.ID);
  return texture;
}

void textureLoad(Texture* texture, const char* path, int wrap, int minFilter, int maxFilter, int colorType) {
  glActiveTexture(texture->unit);
  glBindTexture(GL_TEXTURE_2D, texture->ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);
  stbi_set_flip_vertically_on_load(1);
  int w, h, channels, needsFree;
  unsigned char* data = loadTextureData(path, &w, &h, &channels, &needsFree);
  if (!data) {
    printf("ERROR::TEXTURE::LOAD_FAILED: %s\n", path);
  }
  glTexImage2D(GL_TEXTURE_2D, 0, colorType, w, h, 0, colorType, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  if (needsFree) stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void textureUse(Texture* texture) {
  glActiveTexture(texture->unit);
  glBindTexture(GL_TEXTURE_2D, texture->ID);
}

void textureDelete(Texture* texture) {
  glDeleteTextures(1, &texture->ID);
}
