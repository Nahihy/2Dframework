#pragma once
#include "database/database.h"
#include <2Dframework/entity.h>
#include <2Dframework/world.h>

#define STAND_ANIM 0
#define WALK_ANIM 1
#define JUMP_ANIM 2
#define TOTAL_ANIM_SIZE 3

typedef struct {
  Entity entity;
  TexColumn animations[TOTAL_ANIM_SIZE];
  int animationDelay;
  int delayToNextTex;
  Database savedData;
} Player;


Player createPlayer(const char* image, int colorType, int animationDelay, float maxVelocity,
                    float accelaration, float jumpPower, float modelSize[2], TexColumn standAnim,
                    TexColumn walkAnim, TexColumn jumpAnim, float xCoord, float yCoord, float width, float height);
void playerDelete(Player* player);
void playerDraw(Player* player);
void playerGetUserMovement(Player* player, Randerer* randerer, World* world);
void playerSendPlayerToSpawn(Player* player, World* world);
void playerSaveLocation(Player* player);
void playerSendToLastSavedLocation(Player* player);
