#include <2Dframework/world.h>

World createWorld(Background bg, Ground ground, int gravityLevel[2],
                  float playerSpawn[2], float bgMovementWithGround, float border[4]) {
  World world;
  world.bg = bg;
  world.ground = ground;
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
  groundDelete(&world->ground);
}


void worldDraw(World* world) {
  backgroundDraw(&world->bg);
  groundDraw(&world->ground);
} 

void worldMove(World* world, float horizontal, float vertical) {
  backgroundMove(&world->bg, -horizontal * world->bgMovementWithGround, -vertical * world->bgMovementWithGround);
  groundMove(&world->ground, horizontal, vertical);
  world->playerSpawn[0] += horizontal;
  world->playerSpawn[1] += vertical;
}

void worldZoom(World* world, float level) {
  backgroundZoom(&world->bg, level);
  groundZoom(&world->ground, level);
}

void worldSetScale(World* world, float scale) {
  backgroundSetScale(&world->bg, scale);
  groundSetScale(&world->ground, scale);
}
