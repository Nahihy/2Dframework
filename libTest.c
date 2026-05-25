#include "2Dframework/background.h"
#include "2Dframework/world.h"
#include <2Dframework/2Dframework.h>

int main() {

  Randerer randerer = randererInit("2DframeworkTest", (int[]){800, 600});
  randererSetAutoFrameResizingKeepRatio(&randerer);

  World world = createWorld(createBackground("background.png", GL_RGBA, 0.0f, 0.2f, 0.2f, 0.0f, BG_REPEAT),
                            1024, (int[2]){0, 2}, (float[2]){0.0f, 0.1f}, 0.1f);
  
  groundAdd(&world.ground, "brick.png", GL_RGBA, GO_SQUARE,  1.0f,  -0.6f, 0.1f, 0.1f, 0.0f);
  groundAdd(&world.ground, "brick.png", GL_RGBA, GO_SQUARE, -1.0f,  2.0f, 0.1f, 0.1f, 0.0f);
  for(float i = -15.0f; i < 15.0f; i += 0.2f) 
    groundAdd(&world.ground, "brick.png", GL_RGBA, GO_SQUARE, i, -0.8f, 0.1f, 0.1f, 0.0f);
  for(float i = -0.8f; i < 5.0f; i += 0.2f) 
    groundAdd(&world.ground, "brick.png", GL_RGBA, GO_SQUARE, -15.0f, i, 0.1f, 0.1f, 0.0f);
  for(float i = -0.8f; i < 5.0f; i += 0.2f) 
    groundAdd(&world.ground, "brick.png", GL_RGBA, GO_SQUARE, 15.0f, i, 0.1f, 0.1f, 0.0f);
  for(int i = 1; i < 30; i++) {
    for(int j = 1; j <= i; j++) {
      groundAdd(&world.ground, "brick.png", GL_RGBA, GO_SQUARE, 0.6f + 0.4f * i, 0.2f * j - 1.0f, 0.1f, 0.1f, 0.0f);
    }
  }
  

  Player player = createPlayer("mario.png", GL_RGBA, 20, 0.5f, 0.04f, 0.2f, (float[2]){0.3333f, 0.3333f},
                               (TexColumn){0, 1, EN_REPEAT}, (TexColumn){1, 3, EN_REPEAT},
                               (TexColumn){2, 1, EN_REPEAT}, 0.0f, 0.01f, 0.15f, 0.15f);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  playerSendPlayerToSpawn(&player, &world);
  while(!randererShouldClose(&randerer)) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    worldDraw(&world);
    
    playerDraw(&player);
    playerGetUserMovement(&player, &randerer, &world);
    worldZoom(&world, 1.001f);
    entityZoom(&player.entity, 1.001);
    randererSwapBuffers(&randerer);
  }
  worldDelete(&world);
  playerDelete(&player);
  randererClose(&randerer);

}
