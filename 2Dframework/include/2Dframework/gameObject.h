#pragma once
#include <randerer/randerer.h>

#define TDF_SQUARE createSquareMeshTemp()
#define TDF_TRIANGLE createTriangleMeshTemp()


typedef struct {
  Sprite sprite;
  float xCoord, yCoord;
  float width, height;
  float rotation;
  float horiTexOffset, vertTexOffset;
  float scale;
  float baseWidth, baseHeight;
  float baseXCoord, baseYCoord;
} GameObject;

GameObject createGameObject(const char* image, GLenum colorType, GLenum texWrap, Mesh mesh,
                            float xCoord, float yCoord, float width, float height, float rotation);
void gameObjectDelete(GameObject* object);
void gameObjectDraw(GameObject* object);
void gameObjectMove(GameObject* object, float horizontal, float vertical);
void gameObjectResize(GameObject* object, float horizontal, float vertical);
void gameObjectSetSize(GameObject* object, float height, float width);
void gameObjectRotate(GameObject* object, float rotation);
void gameObjectChangeTexFile(GameObject* object, const char* image, GLenum colorType);
void gameObjectMoveTex(GameObject* object, float horizontal, float vertical);
void gameObjectUpdateTex(GameObject* object);
void gameObjectUpdate(GameObject* object);
int gameObjectCheckCollision(GameObject* object1, GameObject* object2);
void gameObjectSetLocation(GameObject* object, float xCoord, float yCoord);
void gameObjectSetScale(GameObject* object, float scale);
void gameObjectZoom(GameObject* object, float level);
