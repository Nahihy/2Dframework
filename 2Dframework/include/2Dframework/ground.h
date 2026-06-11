#pragma once
#include <2Dframework/gameObject.h>

#define MAX_TEX_NAME_SIZE 128

typedef struct {
  GameObject* objArray;
  int objCount;
  int maxCount;
  float friction;
  char texture[MAX_TEX_NAME_SIZE];
  int colorType;
} Ground;


Ground createGround(int initialCount, const char texture[MAX_TEX_NAME_SIZE], int colorType, float friction);
void groundAdd(Ground* ground, float xCoord, float yCoord, float width, float height, float rotation);
void groundEnlarge(Ground* ground, int amount);
void groundDeleteInSlot(Ground* ground, int slot);
void groundDelete(Ground* ground);
void groundAddToSlot(Ground* ground, int slot);
void groundDraw(Ground* ground);
int groundCheckCollision(Ground* ground, GameObject* object);
void groundMove(Ground* ground, float horizontal, float vertical);
void groundZoom(Ground* ground, float level);
void groundSetScale(Ground* ground, float scale);
