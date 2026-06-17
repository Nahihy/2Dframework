#pragma once
#include <randerer/randerer.h>

typedef enum {
  REPEAT = GL_REPEAT,
  MIRROR = GL_MIRRORED_REPEAT
} BGmode;

typedef struct {
  Sprite sprite;
  vec2 texOffset;
  float scale;
} Background;

Background createBackground(const char* image, GLenum colorType, float left, float right, float up, float down, BGmode bgMode);
void backgroundDraw(Background* background);
void backgroundDelete(Background* background);
void backgroundMove(Background* background, float horizontal, float vertical);
void backgroundUpdate(Background* background);
void backgroundZoom(Background* background, float level);
void backgroundSetScale(Background* background, float scale);
