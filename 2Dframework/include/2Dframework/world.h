#pragma once
#include <2Dframework/ground.h>
#include <2Dframework/background.h>

typedef struct {
  Background bg;
  Ground* groundArray;
  int groundAmount;
  float gravityLevel[2];
  float playerSpawn[2];
  float basePlayerSpawn[2];
  float bgMovementWithGround;
  float border[4];
} World;

World createWorld(Background bg, Ground* groundArray, int groundAmount, float gravityLevel[2],
                  float playerSpawn[2], float bgMovementWithGround, float border[4]);
void worldDelete(World* world);
void worldDraw(World* world);
void worldMove(World* world, float horizontal, float vertical);
void worldZoom(World* world, float level);
void worldSetScale(World* world, float scale);
int worldCheckCollision(World* world, GameObject* object);
