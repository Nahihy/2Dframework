#pragma once
#include <2Dframework/gameObject.h>


typedef struct {
  GameObject* objArray;
  int objCount;
  int maxCount;
} Ground;


Ground createGround(int initialCount);
void groundAdd(Ground* ground, const char* image, int colorType, Mesh mesh,
               float xCoord, float yCoord, float width, float height, float rotation);
void groundEnlarge(Ground* ground, int amount);
void groundDeleteInSlot(Ground* ground, int slot);
void groundDelete(Ground* ground);
void groundAddToSlot(Ground* ground, int slot);
void groundDraw(Ground* ground);
int groundCheckCollision(Ground* ground, GameObject* object);
void groundMove(Ground* ground, float horizontal, float vertical);
void groundZoom(Ground* ground, float mult);
