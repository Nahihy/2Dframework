#include <randerer/randerer.h>

#include <stdio.h>

Randerer randererInit(const char *windowTitle, int windowDimensions[2]) {
  Randerer randerer;
  randerer.lastFrame = 0.0f;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  randerer.window = createWindow(windowTitle, windowDimensions);
  glfwMakeContextCurrent(randerer.window.GLFWwindow);
  if (randerer.window.GLFWwindow == NULL) {
    printf("Failed to create GLFW window\n");
    glfwTerminate();
  }
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD\n");
  }

  windowSetViewPoint(&randerer.window);
  return randerer;
}

void randererClose(Randerer* randerer) {
  windowDelete(&randerer->window);
  glfwTerminate();
}

float randererGetDeltaTime(Randerer* randerer) {
  float currentFrame = glfwGetTime();
  float deltaTime = currentFrame - randerer->lastFrame;
  randerer->lastFrame = currentFrame;
  return deltaTime;
}

Sprite createSprite(Mesh* mesh, Shader* shader, Texture* texture) {
  Sprite sprite;

  sprite.mesh = *mesh;
  sprite.shader = *shader;
  if(texture != NULL) {
    sprite.texture = *texture;
    sprite.isTextured = 1;
  }
  else sprite.isTextured = 0;
  return sprite;
}


void spriteDraw(Sprite* sprite) {
  if(sprite->isTextured) textureUse(&sprite->texture);
  shaderUse(&sprite->shader);
  meshDraw(&sprite->mesh);
}

void spriteDelete(Sprite* sprite) {
  meshDelete(&sprite->mesh);
  shaderDelete(&sprite->shader);
  if(sprite->isTextured) textureDelete(&sprite->texture);
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

