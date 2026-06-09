#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <randerer/mesh.h>
#include <randerer/shader.h>
#include <randerer/window.h>
#include <randerer/texture.h>

typedef struct {
  Shader shader;
  Mesh mesh;
  Texture texture;
  int isTextured;
} Sprite;


typedef struct {
  float lastFrame;
  float deltaTime;
  Window window;
} Randerer;

Randerer randererInit(const char* windowTitle, int windowDimensions[2]);
void randererClose(Randerer* randerer);
void randererUpdateDeltaTime(Randerer* randerer);

int inline randererShouldClose(Randerer* randerer) {
  return windowShouldclose(&randerer->window);
}

static void inline randererSwapBuffers(Randerer* randerer) {
  windowSwapBuffers(&randerer->window);
}

static void inline randererSetAutoFrameResizing(Randerer* randerer) {
  glfwSetWindowUserPointer(randerer->window.GLFWwindow, &randerer->window);
  windowSetAutoFrameResizing(&randerer->window);
}

static void inline randererSetAutoFrameResizingKeepRatio(Randerer* randerer) {
  glfwSetWindowUserPointer(randerer->window.GLFWwindow, &randerer->window);
  windowSetAutoFrameResizingKeepRatio(&randerer->window);
}


Sprite createSprite(Mesh* mesh, Shader* shader, Texture* texture);
void spriteDraw(Sprite* sprite);
void spriteDelete(Sprite* sprite);
