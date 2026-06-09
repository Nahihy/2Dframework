#pragma once
#include <2Dframework/gameObject.h>
#include <2Dframework/ground.h>
#include <2Dframework/world.h>

#define EN_USE_COLLISION 0
#define EN_IGNORE_COLLISION 1
#define EN_REPEAT 2
#define EN_MIRROR 3

typedef enum {
  LEFT = -1,
  RIGHT = 1
} Direction;

typedef struct {
  int column;
  int count;
  int actionAtEnd;
} TexColumn;

typedef struct {
  float modelsize[2];
  TexColumn* modelColumns;
  int modelColumnCount;
  int currModelColumn;
  int currModel;
  Direction side;
} ModelAttrib;

typedef struct {
  GameObject obj;
  int ignoreCollision;
  ModelAttrib model;
  float maxVelocity;
  float accelaration;
  float currHoriVelocity;
  float currVertVelocity;
  int isOnGround;
  float currJumpAccel;
  int delayToNextTex;
  float jumpPower;
  float collisionStep;
  float baseCollisionStep;
  float xWorldCoord;
  float yWorldCoord;
} Entity;



Entity createEntity(const char* image, int colorType, ModelAttrib* model, int ignoreCollision, float accelaration,
                    float maxVelocity, float jumpPower, float xCoord, float yCoord, float width, float height);
void entityDelete(Entity* entity);
void entityDraw(Entity* entity);
void entityResize(Entity* entity, float horizontal, float vertical);
void entityChangeTexColumn(Entity* entity, int column);
void entityNextTex(Entity* entity);
void entityUpdateTex(Entity* entity);
void entityUpdateMovement(Entity* entity, float horiMovement, float vertMovement, World* world);
void entitySwitchToSide(Entity* entity, Direction side);
void entityZoom(Entity* entity, float level);
void entitySetScale(Entity* entity, float scale);
void entityJump(Entity* entity, Randerer* randerer, World* world);
Mesh createEntityMesh(float texCoord[2]);
