#include <2Dframework/gameObject.h>

GameObject createGameObject(const char* image, int colorType, int texWrap, Mesh mesh, float xCoord, float yCoord, float width, float height, float rotation) {
  GameObject object;

  object.xCoord = xCoord;
  object.yCoord = yCoord;
  object.width = width;
  object.height = height;
  object.rotation = rotation;
  object.scale = 1.0f;
  object.baseWidth = width;
  object.baseHeight = height;
  object.baseXCoord = xCoord;
  object.baseYCoord = yCoord;
  object.horiTexOffset = 0.0f;
  object.vertTexOffset = 0.0f;
  
  Shader shader = createShader("gameObject/vertex.glsl", "gameObject/fragment.glsl");

  mat4 trans;
  glm_mat4_identity(trans);
  glm_translate(trans, (vec3){object.xCoord, object.yCoord, 0.0f});
  glm_scale(trans, (vec3){object.width, object.height, 0.0f});
  glm_rotate(trans, glm_rad(object.rotation), (vec3){0.0f, 0.0f, 1.0f});
  shaderSetMat4(&shader, "transform", trans);
  shaderSetInt(&shader, "tex", 1);

  Texture texture = createTexture(GL_TEXTURE1);
  textureLoad(&texture, image, texWrap, GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST, colorType);
  object.sprite = createSprite(&mesh, &shader, &texture);

  return object;
}


void gameObjectDraw(GameObject* object) {
  spriteDraw(&object->sprite);
}

void gameObjectDelete(GameObject* object) {
  spriteDelete(&object->sprite);
}

void gameObjectMove(GameObject* object, float horizontal, float vertical) {
  object->baseXCoord += horizontal;
  object->baseYCoord += vertical;
  object->xCoord = object->baseXCoord * object->scale;
  object->yCoord = object->baseYCoord * object->scale;
  gameObjectUpdate(object);
}

void gameObjectSetLocation(GameObject* object, float xCoord, float yCoord) {
  object->baseXCoord = xCoord;
  object->baseYCoord = yCoord;
  object->xCoord = object->baseXCoord * object->scale;
  object->yCoord = object->baseYCoord * object->scale;  
  gameObjectUpdate(object);
}

void gameObjectResize(GameObject* object, float horizontal, float vertical) {
  object->baseWidth += horizontal;
  object->baseHeight += vertical;
  object->width = object->baseWidth * object->scale;
  object->height = object->baseHeight * object->scale;
  gameObjectUpdate(object);
}

void gameObjectSetSize(GameObject* object, float height, float width) {
  object->baseWidth = width;
  object->baseHeight = height;
  object->width = object->baseWidth * object->scale;
  object->height = object->baseHeight * object->scale;  
  gameObjectUpdate(object); 
}

void gameObjectRotate(GameObject* object, float rotation) {
  object->rotation += rotation;
  gameObjectUpdate(object);
}

void gameObjectUpdate(GameObject* object) {
  mat4 trans;
  glm_mat4_identity(trans);
  glm_translate(trans, (vec3){object->xCoord, object->yCoord, 0.0f});
  glm_scale(trans, (vec3){object->width, object->height, 0.0f});
  glm_rotate(trans, glm_rad(object->rotation), (vec3){0.0f, 0.0f, 1.0f});
  shaderSetMat4(&object->sprite.shader, "transform", trans);
}

void gameObjectChangeTexFile(GameObject* object, const char* image, int colorType) {
  textureLoad(&object->sprite.texture, image, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST, colorType);
}

void gameObjectMoveTex(GameObject* object, float horizontal, float vertical) {
  object->horiTexOffset += horizontal;
  object->vertTexOffset += vertical;
  gameObjectUpdateTex(object);
}

void gameObjectUpdateTex(GameObject* object) {
  shaderSetVec2(&object->sprite.shader, "texOffset", (vec2){object->horiTexOffset, object->vertTexOffset});
}


int gameObjectCheckCollision(GameObject* object1, GameObject* object2) {

  float one_left = object1->xCoord - object1->width;
  float one_right = object1->xCoord + object1->width;
  float one_bottom = object1->yCoord - object1->height;
  float one_top = object1->yCoord + object1->height;
  
  float two_left = object2->xCoord - object2->width;
  float two_right = object2->xCoord + object2->width;
  float two_bottom = object2->yCoord - object2->height;
  float two_top = object2->yCoord + object2->height;
  
  return 
    one_right >= two_left && two_right >= one_left &&
    one_top >= two_bottom && two_top >= one_bottom;
}

void gameObjectSetScale(GameObject* object, float scale) {
  object->scale = scale;
  object->width = object->baseWidth * object->scale;
  object->height = object->baseHeight * object->scale;
  object->xCoord = object->baseXCoord * object->scale;
  object->yCoord = object->baseYCoord * object->scale;
  gameObjectUpdate(object);
}
void gameObjectZoom(GameObject* object, float level) {
  object->scale += level;
  if(object->scale < 0.0f) object->scale -= level;
  object->width = object->baseWidth * object->scale;
  object->height = object->baseHeight * object->scale;
  object->xCoord = object->baseXCoord * object->scale;
  object->yCoord = object->baseYCoord * object->scale;
  gameObjectUpdate(object);
}
