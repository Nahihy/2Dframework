#include <2Dframework/world.h>
#include <stdlib.h>

World createWorld(Background bg, Ground* groundArray, int groundAmount, float gravityLevel[2],
                  float playerSpawn[2], float bgMovementWithGround, float border[4]) {
  World world;
  world.bg = bg;
  world.groundArray = malloc(groundAmount * sizeof(Ground));
  for(int i = 0; i < groundAmount; i++) world.groundArray[i] = groundArray[i];
  world.groundAmount = groundAmount;
  world.gravityLevel[0] = gravityLevel[0];
  world.gravityLevel[1] = gravityLevel[1];
  world.playerSpawn[0] = playerSpawn[0];
  world.playerSpawn[1] = playerSpawn[1];
  world.basePlayerSpawn[0] = playerSpawn[0];
  world.basePlayerSpawn[1] = playerSpawn[1];
  world.bgMovementWithGround = bgMovementWithGround;
  world.border[0] = border[0];
  world.border[1] = border[1];
  world.border[2] = border[2];
  world.border[3] = border[3];

  return world;
}

void worldDelete(World* world) {
  backgroundDelete(&world->bg);
  for(int i = 0; i < world->groundAmount; i++) groundDelete(&world->groundArray[i]);
}


void worldDraw(World* world) {
  backgroundDraw(&world->bg);
  for(int i = 0; i < world->groundAmount; i++) groundDraw(&world->groundArray[i]);
} 

void worldMove(World* world, float horizontal, float vertical) {
  backgroundMove(&world->bg, -horizontal * world->bgMovementWithGround, -vertical * world->bgMovementWithGround);
  for(int i = 0; i < world->groundAmount; i++) groundMove(&world->groundArray[i], horizontal, vertical);
  world->playerSpawn[0] += horizontal;
  world->playerSpawn[1] += vertical;
}

void worldZoom(World* world, float level) {
  backgroundZoom(&world->bg, level);
  for(int i = 0; i < world->groundAmount; i++) groundZoom(&world->groundArray[i], level);
}

void worldSetScale(World* world, float scale) {
  backgroundSetScale(&world->bg, scale);
  for(int i = 0; i < world->groundAmount; i++) groundSetScale(&world->groundArray[i], scale);
}

int worldCheckCollision(World* world, GameObject* object) {
  for(int i = 0; i < world->groundAmount; i++) 
    if(groundCheckCollision(&world->groundArray[i], object))
      return i;
  return -1;
}
