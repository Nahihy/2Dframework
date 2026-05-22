#include <randerer/window.h>
#include <stdio.h>

Window createWindow(const char* windowTitle, int windowDimensions[2]) {
  Window window;
  window.windowTitle = windowTitle;
  window.windowDimensions[0] = windowDimensions[0];
  window.windowDimensions[1] = windowDimensions[1];
  window.GLFWwindow = glfwCreateWindow(window.windowDimensions[0], window.windowDimensions[1], windowTitle, NULL, NULL);
  if(window.GLFWwindow == NULL) {
    printf("Failed to create GLFW window\n");
    glfwTerminate();
  }
  return window;
}

void windowMakeCurrent(Window* window) {
  glfwMakeContextCurrent(window->GLFWwindow);
  glfwSetWindowUserPointer(window->GLFWwindow, window);
}

void windowDelete(Window* window) {
  glfwDestroyWindow(window->GLFWwindow);
  window->GLFWwindow = NULL;
}

void windowSetViewPoint(Window* window) {
  glViewport(0, 0, window->windowDimensions[0], window->windowDimensions[1]);
}

void frameBufferSize_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}
void frameBufferSizeKeepRatio_callback(GLFWwindow* window, int width, int height) {
  Window* windowClass = glfwGetWindowUserPointer(window);
  float window_aspect = (float)width / (float)height;
  
  int render_width, render_height;
  int offset_x = 0, offset_y = 0;
  
  if (window_aspect > ((float)windowClass->windowDimensions[0] / (float)windowClass->windowDimensions[1])) {
      render_height = height;
      render_width = (int)(height * ((float)windowClass->windowDimensions[0] / (float)windowClass->windowDimensions[1]));
      offset_x = (width - render_width) / 2;
  } else {
      render_width = width;
      render_height = (int)(width / ((float)windowClass->windowDimensions[0] / (float)windowClass->windowDimensions[1]));
      offset_y = (height - render_height) / 2;
  }
  
  glViewport(offset_x, offset_y, render_width, render_height);
  windowClass->windowDimensions[0] = render_width;
  windowClass->windowDimensions[1] = render_height;
}


// void frameBufferSizeKeepRatioPreferHeight_callback(GLFWwindow* window, int width, int height) {
//   Window* windowClass = glfwGetWindowUserPointer(window);
//
//   int render_height = width;
//   int render_width = (int)(height * ((float)windowClass->windowDimensions[0] / (float)windowClass->windowDimensions[1]));
//   
//   glViewport(0, 0, render_width, render_height);
//   windowClass->windowDimensions[0] = render_width;
//   windowClass->windowDimensions[1] = render_height;
// }
// void frameBufferSizeKeepRatioPreferWidth_callback(GLFWwindow* window, int width, int height) {
//   Window* windowClass = glfwGetWindowUserPointer(window);
//
//   int render_width = width;
//   int render_height = (int)(width / ((float)windowClass->windowDimensions[0] / (float)windowClass->windowDimensions[1]));
//   
//   glViewport(0, 0, render_width, render_height);
//   windowClass->windowDimensions[0] = render_width;
//   windowClass->windowDimensions[1] = render_height;
// }
