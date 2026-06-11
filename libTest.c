#include <2Dframework/2Dframework.h>

void getZoomControl(Player* player, World* world, Randerer* randerer) {
  if(glfwGetKey(randerer->window.GLFWwindow, GLFW_KEY_MINUS) == GLFW_PRESS) {
    worldZoom(world, -0.05f);
    entityZoom(&player->entity, -0.05f);
  }
  if(glfwGetKey(randerer->window.GLFWwindow, GLFW_KEY_EQUAL) == GLFW_PRESS) {
    worldZoom(world, 0.05f);
    entityZoom(&player->entity, 0.05f);
  }
}


int main() {

  Randerer randerer = randererInit("2DframeworkTest", (int[]){800, 600});
  randererSetAutoFrameResizingKeepRatio(&randerer);

  World world = createWorld(createBackground("background.png", GL_RGBA, 0.0f, 0.2f, 0.2f, 0.0f, BG_REPEAT),
                            (Ground[]){createGround(1024, "brick.png", GL_RGBA, 0.6f), 
                                       createGround(64, "ice.png", GL_RGBA, 0.95f)}, 2,
                            (float[2]){0.0f, 1.0f}, (float[2]){0.0f, 0.1f}, 0.1f, (float[4]){-20.0f, 20.0f, 100.0f, -10.0f});
  
  groundAdd(&world.groundArray[1], -1.0f,  0.4f, 0.1f, 0.1f, 0.0f);
  for(float i = -15.0f; i < -7.0f; i += 0.2f) 
    groundAdd(&world.groundArray[1], i, -0.8f, 0.1f, 0.1f, 0.0f);
  for(float i = -7.0f; i < 1.0f; i += 0.2f) 
    groundAdd(&world.groundArray[0], i, -0.8f, 0.1f, 0.1f, 0.0f);
  for(float i = -0.8f; i < 5.0f; i += 0.2f) 
    groundAdd(&world.groundArray[0], -15.0f, i, 0.1f, 0.1f, 0.0f);
  for(int i = 1; i < 30; i++)
    for(int j = 1; j <= i; j++)
      groundAdd(&world.groundArray[0], 0.6f + 0.4f * i, 0.2f * j - 1.0f, 0.1f, 0.1f, 0.0f);
  

  Player player = createPlayer("mario.png", GL_RGBA, 20, 3.0f, 1.0f, 4.0f, (float[2]){0.3333f, 0.3333f},
                               (TexColumn){0, 1, EN_REPEAT}, (TexColumn){1, 3, EN_REPEAT},
                               (TexColumn){2, 1, EN_REPEAT}, 0.0f, 0.01f, 0.095f, 0.095f);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  playerSendPlayerToSpawn(&player, &world);
  playerSendToLastSavedLocation(&player);
  while(!randererShouldClose(&randerer)) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    randererUpdateDeltaTime(&randerer);
    worldDraw(&world);
    
    playerDraw(&player);
    playerGetUserMovement(&player, &randerer, &world);
    getZoomControl(&player, &world, &randerer);

    if(glfwGetKey(randerer.window.GLFWwindow, GLFW_KEY_S) == GLFW_PRESS) 
      playerSaveLocation(&player);

    randererSwapBuffers(&randerer);
  }
  worldDelete(&world);
  playerDelete(&player);
  randererClose(&randerer);

}
