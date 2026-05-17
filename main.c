#include <2Dframework/2Dframework.h>

int main() {

  Randerer randerer = randererInit("2DframeworkTest", (int[]){800, 600});
  randererSetAutoFrameResizingKeepRatio(&randerer);

  World world = createWorld(createBackground("purpleMountsBackground.jpg", GL_RGB, 0.0f, 1.0f, 1.0f, 0.0f, BG_REPEAT),
                            8, (int[2]){0, 1}, (float[2]){0.0f, 0.1f});
  
  groundAdd(&world.ground, "container.jpg", GL_RGB, GO_SQUARE,  1.0f,  0.0f, 0.2f, 0.2f, 0.0f);
  groundAdd(&world.ground, "container.jpg", GL_RGB, GO_SQUARE, -1.0f,  0.0f, 0.2f, 0.2f, 0.0f);
  groundAdd(&world.ground, "container.jpg", GL_RGB, GO_SQUARE, -1.0f, -0.4f, 0.2f, 0.2f, 0.0f);
  groundAdd(&world.ground, "container.jpg", GL_RGB, GO_SQUARE, -0.6f, -0.4f, 0.2f, 0.2f, 0.0f);
  groundAdd(&world.ground, "container.jpg", GL_RGB, GO_SQUARE, -0.2f, -0.4f, 0.2f, 0.2f, 0.0f);
  groundAdd(&world.ground, "container.jpg", GL_RGB, GO_SQUARE,  0.2f, -0.4f, 0.2f, 0.2f, 0.0f);
  groundAdd(&world.ground, "container.jpg", GL_RGB, GO_SQUARE,  0.6f, -0.4f, 0.2f, 0.2f, 0.0f);
  groundAdd(&world.ground, "container.jpg", GL_RGB, GO_SQUARE,  1.0f, -0.4f, 0.2f, 0.2f, 0.0f);

  Player player = createPlayer("mario.png", GL_RGBA, 20, 1.5f, 0.1f, (float[2]){0.3333f, 0.3333f}, (TexColumn){0, 1, EN_REPEAT},
                               (TexColumn){1, 3, EN_REPEAT}, (TexColumn){2, 1, EN_REPEAT}, 0.0f, 0.01f, 0.2f, 0.2f);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  
  playerSendPlayerToSpawn(&player, &world);

  while(!randererShouldClose(&randerer)) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    worldDraw(&world);

    playerDraw(&player);
    playerGetUserMovement(&player, &randerer, &world);
    randererSwapBuffers(&randerer);
  }
  worldDelete(&world);
  playerDelete(&player);
  randererClose(&randerer);

}
