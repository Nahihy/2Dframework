#include "2Dframework/gameObject.h"
#include <2Dframework/ground.h>
#include <stdlib.h>
#include <string.h>


Ground createGround(int initialCount, const char texture[MAX_TEX_NAME_SIZE], int colorType, float slipperiness) {
  Ground ground;
  ground.objArray = malloc(initialCount * sizeof(GameObject));
  ground.objCount = 0;
  ground.maxCount = initialCount;
  strcpy(ground.texture, texture);
  ground.colorType = colorType;
  ground.slipperiness = slipperiness;

  return ground;
}

void groundAdd(Ground* ground, float xCoord, float yCoord, float width, float height, float rotation) {
  if(ground->maxCount <= ground->objCount)
    groundEnlarge(ground, 1);
  ground->objArray[ground->objCount] = createGameObject(ground->texture, ground->colorType, GL_REPEAT, GO_SQUARE,
                                                        xCoord, yCoord, width, height, rotation);
  ground->objCount++;
}

void groundEnlarge(Ground* ground, int amount) {
  ground->maxCount += amount;
  ground->objArray = realloc(ground->objArray, ground->maxCount * sizeof(GameObject));
}

void groundDelete(Ground* ground) {
  for(int i = 0; i < ground->objCount; i++)
    gameObjectDelete(&ground->objArray[i]);
  free(ground->objArray);
}

void groundDeleteInSlot(Ground* ground, int slot) {
  gameObjectDelete(&ground->objArray[slot]);
}

void groundDraw(Ground* ground) {
  for(int i = 0; i < ground->objCount; i++)
    gameObjectDraw(&ground->objArray[i]);
}

int groundCheckCollision(Ground* ground, GameObject* object) {
  for(int i = 0; i < ground->objCount; i++)
    if(gameObjectCheckCollision(&ground->objArray[i], object)) return 1;
  return 0;
}

void groundMove(Ground* ground, float horizontal, float vertical) {
  for(int i = 0; i < ground->objCount; i++)
    gameObjectMove(&ground->objArray[i], horizontal, vertical);
}

void groundZoom(Ground* ground, float level) {
  for(int i = 0; i < ground->objCount; i++)
    gameObjectZoom(&ground->objArray[i], level);
}

void groundSetScale(Ground* ground, float scale) {
  for(int i = 0; i < ground->objCount; i++)
    gameObjectSetScale(&ground->objArray[i], scale);
}
