#pragma once

#include <glad/glad.h>
#include <cglm/cglm.h>

typedef enum {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
} Camera_Movement;

#define CAMERA_YAW         -90.0f
#define CAMERA_PITCH         0.0f
#define CAMERA_SPEED         2.5f
#define CAMERA_SENSITIVITY   0.1f
#define CAMERA_ZOOM         45.0f

typedef struct {
  vec3 position;
  vec3 front;
  vec3 up;
  vec3 right;
  vec3 worldUp;
  float yaw;
  float pitch;
  float movementSpeed;
  float mouseSensitivity;
  float zoom;
} Camera;

Camera createCamera(vec3 position, vec3 up, float yaw, float pitch);
void cameraGetViewMatrix(Camera* camera, mat4 dest);
void cameraProcessKeyboard(Camera* camera, Camera_Movement direction, float deltaTime);
void cameraProcessMouseMovement(Camera* camera, float xoffset, float yoffset, int constrainPitch);
void cameraProcessMouseScroll(Camera* camera, float yoffset);

