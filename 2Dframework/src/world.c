#include <2Dframework/world.h>

World createWorld(Background bg, int initGroundCount, int gravityLevel[2], float playerSpawn[2], float bgMoveWithGround) {
  World world;
  world.bg = bg;
  world.ground = createGround(initGroundCount);
  world.gravityLevel[0] = gravityLevel[0];
  world.gravityLevel[1] = gravityLevel[1];
  world.playerSpawn[0] = playerSpawn[0];
  world.playerSpawn[1] = playerSpawn[1];
  world.bgMoveWithGround = bgMoveWithGround;

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
  backgroundMove(&world->bg, horizontal * world->bgMoveWithGround, vertical * world->bgMoveWithGround);
  groundMove(&world->ground, horizontal, vertical);
}
