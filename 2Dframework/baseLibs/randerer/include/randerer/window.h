#pragma once
#include <GLFW/glfw3.h>

typedef struct {
  GLFWwindow* GLFWwindow;
  const char* windowTitle;
  int windowDimensions[2];
} Window;

Window createWindow(const char* windowTitle, int windowDimensions[2]);
void windowMakeCurrent(Window* window);
void windowSetViewPoint(Window* window);
void windowDelete(Window* window);

void frameBufferSize_callback(GLFWwindow* window, int width, int height);
void frameBufferSizeKeepRatio_callback(GLFWwindow* window, int width, int height);

int inline windowShouldclose(Window* window) {
  return glfwWindowShouldClose(window->GLFWwindow);
}
static void inline windowSwapBuffers(Window* window) {
  glfwSwapBuffers(window->GLFWwindow);
  glfwPollEvents();
}
static void inline windowSetAutoFrameResizing(Window* window) {
  glfwSetFramebufferSizeCallback(window->GLFWwindow, frameBufferSize_callback);
}
static void inline windowSetAutoFrameResizingKeepRatio(Window* window) {
  glfwSetFramebufferSizeCallback(window->GLFWwindow, frameBufferSizeKeepRatio_callback);
}
