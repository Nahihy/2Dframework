#include "2Dframework/entity.h"
#include <2Dframework/2Dframework.h>

Player createPlayer(const char* image, int colorType, int animationDelay, float maxVelocity, float accelaration, float modelSize[2],
                    TexColumn standAnim, TexColumn walkAnim, TexColumn jumpAnim, float xCoord, float yCoord, float width, float height) {
  Player player;
  player.animationDelay = animationDelay;
  player.delayToNextTex = animationDelay;
  player.entity.ignoreCollision = EN_USE_COLLISION;
  player.entity.model.modelsize[0] = modelSize[0];
  player.entity.model.modelsize[1] = modelSize[1];
  player.entity.model.modelColumns = malloc(TOTAL_ANIM_SIZE * sizeof(TexColumn));
  player.entity.model.modelColumns[STAND_ANIM] = standAnim;
  player.entity.model.modelColumns[WALK_ANIM] = walkAnim;
  player.entity.model.modelColumns[JUMP_ANIM] = jumpAnim;
  player.entity.model.modelColumnCount = TOTAL_ANIM_SIZE;
  player.entity.model.currModel = 1;
  player.entity.model.currModelColumn = STAND_ANIM;
  player.entity.model.side = RIGHT;
  player.entity.maxVelocity = maxVelocity;
  player.entity.accelaration = accelaration;
  player.entity.currHoriVelocity = 0.0f;
  player.entity.currVertVelocity = 0.0f;

  player.entity.obj = createGameObject(image, colorType, GL_MIRRORED_REPEAT, createEntityMesh(player.entity.model.modelsize),  xCoord, yCoord, width, height, 0.0f);
  entityUpdateTex(&player.entity);


  return player;
}

void playerDelete(Player* player) {
  entityDelete(&player->entity);
}

void playerDraw(Player* player) {
  entityDraw(&player->entity);
}

void playerGetUserMovement(Player* player, Randerer* randerer, World* world) {
  int wPressed = glfwGetKey(randerer->window.GLFWwindow, GLFW_KEY_W) == GLFW_PRESS;
  int sPressed = glfwGetKey(randerer->window.GLFWwindow, GLFW_KEY_S) == GLFW_PRESS;
  int dPressed = glfwGetKey(randerer->window.GLFWwindow, GLFW_KEY_D) == GLFW_PRESS;
  int aPressed = glfwGetKey(randerer->window.GLFWwindow, GLFW_KEY_A) == GLFW_PRESS;
  
  entityUpdateMovement(&player->entity, 0.0f, 0.0f, world);

  if(!wPressed && !sPressed && !dPressed && !aPressed) {
    entityChangeTexColumn(&player->entity, STAND_ANIM);
    player->delayToNextTex = player->animationDelay;
    return;
  }


  if(player->delayToNextTex <= 0) {
    entityNextTex(&player->entity);
    player->delayToNextTex = player->animationDelay;
  }
  else player->delayToNextTex--;

  if(wPressed && player->entity.isOnGround) {
    entityUpdateMovement(&player->entity, 0.0f, 5.0f, world);
    player->entity.isOnGround = 0;
    if(player->entity.model.currModelColumn != JUMP_ANIM) 
      entityChangeTexColumn(&player->entity, JUMP_ANIM);
  }  
  if(sPressed) {
    entityUpdateMovement(&player->entity, 0.0f, -1.0f, world);
    if(player->entity.model.currModelColumn != JUMP_ANIM) entityChangeTexColumn(&player->entity, JUMP_ANIM);
  }
  if(dPressed) {
    entityUpdateMovement(&player->entity, 1.0f, 0.0f, world);
    if(player->entity.model.currModelColumn != WALK_ANIM) entityChangeTexColumn(&player->entity, WALK_ANIM);
    entitySwitchToSide(&player->entity, RIGHT);
  }
  if(aPressed) {
    entityUpdateMovement(&player->entity, -1.0f, 0.0f, world);
    if(player->entity.model.currModelColumn != WALK_ANIM) entityChangeTexColumn(&player->entity, WALK_ANIM);
    entitySwitchToSide(&player->entity, LEFT);
  }
}


void playerSendPlayerToSpawn(Player* player, World* world) {
  gameObjectSetLocation(&player->entity.obj, world->playerSpawn[0], world->playerSpawn[1]);
}
