#include "2Dframework/gameObject.h"
#include <2Dframework/ground.h>
#include <stdlib.h>


Ground createGround(int initialMaxCount) {
  Ground ground;
  ground.objArray = malloc(initialMaxCount * sizeof(GameObject));
  ground.objCount = 0;
  ground.maxCount = initialMaxCount;

  return ground;
}

void groundAdd(Ground* ground, const char* image, int colorType, Mesh mesh, float xCoord, float yCoord, float width, float height, float rotation) {
  if(ground->maxCount <= ground->objCount)
    groundEnlarge(ground, 1);
  ground->objArray[ground->objCount] = createGameObject(image, colorType, GL_REPEAT, mesh, xCoord, yCoord, width, height, rotation);
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
