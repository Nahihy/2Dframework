#pragma once
#include <randerer/randerer.h>

#define BG_REPEAT GL_REPEAT
#define BG_MIRROR GL_MIRRORED_REPEAT

typedef struct {
  Sprite sprite;
  vec2 texOffset;
  float texZoom;
} Background;

Background createBackground(const char* image, int colorType, float left, float right, float up, float down, int bgMode);
void backgroundDraw(Background* background);
void backgroundDelete(Background* background);
void backgroundMove(Background* background, float horizontal, float vertical);
void backgroundUpdate(Background* background);
void backgroundZoom(Background* background, float mult);
