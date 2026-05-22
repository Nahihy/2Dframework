#include <randerer/camera.h>
#include <math.h>

static void updateCameraVectors(Camera* camera) {
  vec3 front;
  front[0] = cosf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));
  front[1] = sinf(glm_rad(camera->pitch));
  front[2] = sinf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));
  glm_vec3_normalize_to(front, camera->front);

  glm_vec3_cross(camera->front, camera->worldUp, camera->right);
  glm_vec3_normalize(camera->right);

  glm_vec3_cross(camera->right, camera->front, camera->up);
  glm_vec3_normalize(camera->up);
}

Camera createCamera(vec3 position, vec3 up, float yaw, float pitch) {
  Camera camera;

  glm_vec3_copy(position, camera.position);
  glm_vec3_copy(up, camera.worldUp);
  camera.yaw             = yaw;
  camera.pitch           = pitch;
  camera.movementSpeed   = CAMERA_SPEED;
  camera.mouseSensitivity = CAMERA_SENSITIVITY;
  camera.zoom            = CAMERA_ZOOM;

  // initialize front to default then let updateCameraVectors correct it
  glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, camera.front);

  updateCameraVectors(&camera);
  return camera;
}

void cameraGetViewMatrix(Camera* camera, mat4 dest) {
  vec3 target;
  glm_vec3_add(camera->position, camera->front, target);
  glm_lookat(camera->position, target, camera->up, dest);
}

void cameraProcessKeyboard(Camera* camera, Camera_Movement direction, float deltaTime) {
  float velocity = camera->movementSpeed * deltaTime;
  vec3 tmp;

  if (direction == FORWARD) {
    glm_vec3_scale(camera->front, velocity, tmp);
    glm_vec3_add(camera->position, tmp, camera->position);
  }
  if (direction == BACKWARD) {
    glm_vec3_scale(camera->front, velocity, tmp);
    glm_vec3_sub(camera->position, tmp, camera->position);
  }
  if (direction == LEFT) {
    glm_vec3_scale(camera->right, velocity, tmp);
    glm_vec3_sub(camera->position, tmp, camera->position);
  }
  if (direction == RIGHT) {
    glm_vec3_scale(camera->right, velocity, tmp);
    glm_vec3_add(camera->position, tmp, camera->position);
  }
}

void cameraProcessMouseMovement(Camera* camera, float xoffset, float yoffset, int constrainPitch) {
  xoffset *= camera->mouseSensitivity;
  yoffset *= camera->mouseSensitivity;

  camera->yaw   += xoffset;
  camera->pitch += yoffset;

  if (constrainPitch) {
    if (camera->pitch >  89.0f) camera->pitch =  89.0f;
    if (camera->pitch < -89.0f) camera->pitch = -89.0f;
  }

  updateCameraVectors(camera);
}

void cameraProcessMouseScroll(Camera* camera, float yoffset) {
  camera->zoom -= yoffset;
  if (camera->zoom < 1.0f)  camera->zoom = 1.0f;
  if (camera->zoom > 45.0f) camera->zoom = 45.0f;
}
