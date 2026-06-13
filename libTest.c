#include <2Dframework/2Dframework.h>
#include <stdio.h>

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

void createParkourCourse(World* world);

int main() {

  Randerer randerer = randererInit("2DframeworkTest", (int[]){800, 600});
  randererSetAutoFrameResizingKeepRatio(&randerer);

  World world = createWorld(createBackground("background.png", GL_RGBA, 0.0f, 0.2f, 0.2f, 0.0f, BG_REPEAT),
                            (Ground[3]){createGround(64, "brick.png", GL_RGBA, 0.3f), 
                                       createGround(64, "ice.png", GL_RGBA, 0.05f),
                                       createGround(64, "mud.png", GL_RGBA, 0.6f)}, 3,
                            (float[2]){0.0f, 1.0f}, (float[2]){-14.5f, 2.1f}, 0.1f, (float[4]){-20.0f, 50.0f, 100.0f, -50.0f});
  
  createParkourCourse(&world);

  Player player = createPlayer("mario.png", GL_RGBA, 0.2f, 3.0f, 1.0f, 4.5f, (float[2]){0.3333f, 0.3333f},
                               (TexColumn){0, 1, EN_REPEAT}, (TexColumn){1, 3, EN_REPEAT},
                               (TexColumn){2, 1, EN_REPEAT}, 0.0f, 0.01f, 0.095f, 0.095f);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  playerSendPlayerToSpawn(&player, &world);
  playerSendToLastSavedLocation(&player, &world);
  while(!randererShouldClose(&randerer)) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    randererUpdateDeltaTime(&randerer);
    printf("%.2f\n", randerer.deltaTime);
    worldDraw(&world);
    
    playerDraw(&player);
    playerGetUserMovement(&player, &randerer, &world);
    getZoomControl(&player, &world, &randerer);

    if(glfwGetKey(randerer.window.GLFWwindow, GLFW_KEY_S) == GLFW_PRESS) 
      playerSaveLocation(&player);
    if(glfwGetKey(randerer.window.GLFWwindow, GLFW_KEY_C) == GLFW_PRESS) 
      playerDeleteSavedLocation(&player);
    randererSwapBuffers(&randerer);
  }
  worldDelete(&world);
  playerDelete(&player);
  randererClose(&randerer);

}


void createParkourCourse(World* world) {
  for(float i = -15.0f; i < -12.0f; i += 0.2f)
    groundAdd(&world->groundArray[0], i, -0.8f, 0.1f, 0.1f, 0.0f);
  for(float i = -12.0f; i < -7.0f; i += 0.2f)
    groundAdd(&world->groundArray[1], i, -0.8f, 0.1f, 0.1f, 0.0f);
  for(float i = -7.0f; i < -3.0f; i += 0.2f)
    groundAdd(&world->groundArray[2], i, -0.8f, 0.1f, 0.1f, 0.0f);
  for(float i = -3.0f; i < 1.0f; i += 0.2f)
    groundAdd(&world->groundArray[0], i, -0.8f, 0.1f, 0.1f, 0.0f);
  for(float i = -0.8f; i < 5.0f; i += 0.2f)
    groundAdd(&world->groundArray[0], -15.0f, i, 0.1f, 0.1f, 0.0f);

  // ai genarated parkor course for showcase
  for(int k = 0; k < 3; k++) groundAdd(&world->groundArray[0], 1.4f + k*0.1f, -0.5f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 2; k++) groundAdd(&world->groundArray[1], 2.2f + k*0.1f, -0.2f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 4; k++) groundAdd(&world->groundArray[2], 3.0f + k*0.1f,  0.1f, 0.1f, 0.1f, 0.0f);

  groundAdd(&world->groundArray[0], 3.9f, -0.2f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 3; k++) groundAdd(&world->groundArray[1], 4.4f + k*0.1f,  0.2f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 2; k++) groundAdd(&world->groundArray[2], 5.3f + k*0.1f,  0.5f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 4; k++) groundAdd(&world->groundArray[0], 6.0f + k*0.1f,  0.3f, 0.1f, 0.1f, 0.0f);

  groundAdd(&world->groundArray[1], 7.0f,  0.6f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 3; k++) groundAdd(&world->groundArray[2], 7.6f + k*0.1f,  0.8f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 2; k++) groundAdd(&world->groundArray[0], 8.6f + k*0.1f,  0.4f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 4; k++) groundAdd(&world->groundArray[1], 9.3f + k*0.1f,  0.7f, 0.1f, 0.1f, 0.0f);

  groundAdd(&world->groundArray[2], 10.3f,  1.0f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 3; k++) groundAdd(&world->groundArray[0], 10.9f + k*0.1f,  0.6f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 2; k++) groundAdd(&world->groundArray[1], 11.9f + k*0.1f,  0.9f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 4; k++) groundAdd(&world->groundArray[2], 12.7f + k*0.1f,  0.5f, 0.1f, 0.1f, 0.0f);

  groundAdd(&world->groundArray[0], 13.6f,  0.8f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 3; k++) groundAdd(&world->groundArray[1], 14.1f + k*0.1f,  1.1f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 2; k++) groundAdd(&world->groundArray[2], 15.1f + k*0.1f,  0.7f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 4; k++) groundAdd(&world->groundArray[0], 15.9f + k*0.1f,  1.0f, 0.1f, 0.1f, 0.0f);

  groundAdd(&world->groundArray[1], 16.8f,  0.6f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 3; k++) groundAdd(&world->groundArray[2], 17.5f + k*0.1f,  0.9f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 2; k++) groundAdd(&world->groundArray[0], 18.5f + k*0.1f,  1.2f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 4; k++) groundAdd(&world->groundArray[1], 19.2f + k*0.1f,  0.8f, 0.1f, 0.1f, 0.0f);

  groundAdd(&world->groundArray[2], 20.3f,  1.1f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 3; k++) groundAdd(&world->groundArray[0], 21.0f + k*0.1f,  0.7f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 2; k++) groundAdd(&world->groundArray[1], 21.8f + k*0.1f,  1.0f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 4; k++) groundAdd(&world->groundArray[2], 22.7f + k*0.1f,  1.3f, 0.1f, 0.1f, 0.0f);

  groundAdd(&world->groundArray[0], 23.7f,  0.9f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 3; k++) groundAdd(&world->groundArray[1], 24.5f + k*0.1f,  1.2f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 2; k++) groundAdd(&world->groundArray[2], 25.4f + k*0.1f,  0.8f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 4; k++) groundAdd(&world->groundArray[0], 26.3f + k*0.1f,  1.1f, 0.1f, 0.1f, 0.0f);

  groundAdd(&world->groundArray[1], 27.3f,  0.7f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 3; k++) groundAdd(&world->groundArray[2], 27.9f + k*0.1f,  1.0f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 2; k++) groundAdd(&world->groundArray[0], 28.9f + k*0.1f,  1.3f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 4; k++) groundAdd(&world->groundArray[1], 29.7f + k*0.1f,  0.9f, 0.1f, 0.1f, 0.0f);

  groundAdd(&world->groundArray[2], 30.8f,  1.2f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 3; k++) groundAdd(&world->groundArray[0], 31.5f + k*0.1f,  0.8f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 2; k++) groundAdd(&world->groundArray[1], 32.5f + k*0.1f,  1.1f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 4; k++) groundAdd(&world->groundArray[2], 33.3f + k*0.1f,  0.7f, 0.1f, 0.1f, 0.0f);

  groundAdd(&world->groundArray[0], 34.4f,  1.0f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 5; k++) groundAdd(&world->groundArray[0], 35.0f + k*0.1f, 0.9f, 0.1f, 0.1f, 0.0f);
  for(int k = 0; k < 5; k++) groundAdd(&world->groundArray[1], 35.5f + k*0.1f, 0.9f, 0.1f, 0.1f, 0.0f);
  for(int k = 0; k < 5; k++) groundAdd(&world->groundArray[2], 36.0f + k*0.1f, 0.9f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 5; k++) groundAdd(&world->groundArray[0], 35.0f, 1.0f + k*0.1f, 0.1f, 0.1f, 0.0f);

  for(int k = 0; k < 5; k++) groundAdd(&world->groundArray[0], 36.4f, 1.0f + k*0.1f, 0.1f, 0.1f, 0.0f);

}
