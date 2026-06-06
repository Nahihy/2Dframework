#include <2Dframework/player.h>

Player createPlayer(const char* image, int colorType, int animationDelay, float maxVelocity, float accelaration, float jumpPower, float modelSize[2],
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
  player.entity.currJumpAccel = 0.0f;
  player.entity.jumpPower = jumpPower;
  player.entity.collisionStep = 0.1f;
  player.entity.baseCollisionStep = 0.1f;
  player.entity.isOnGround = 0;

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
  int spacePressed = glfwGetKey(randerer->window.GLFWwindow, GLFW_KEY_SPACE) == GLFW_PRESS;
  int dPressed = glfwGetKey(randerer->window.GLFWwindow, GLFW_KEY_D) == GLFW_PRESS;
  int aPressed = glfwGetKey(randerer->window.GLFWwindow, GLFW_KEY_A) == GLFW_PRESS;

  float totalHoriMovement = 0.0f;
  
  float scale = player->entity.obj.scale;
  float xCoord = player->entity.obj.xCoord;
  float yCoord = player->entity.obj.yCoord;

  worldMove(world, 
          (xCoord >  0.6f ? -(xCoord - 0.6f) / scale : 0.0f) +
          (xCoord < -0.6f ? -(xCoord + 0.6f) / scale : 0.0f),
          (yCoord >  0.6f ? -(yCoord - 0.6f) / scale : 0.0f) +
          (yCoord < -0.6f ? -(yCoord + 0.6f) / scale : 0.0f)
  );

  gameObjectSetLocation(&player->entity.obj, 
          (xCoord >  0.6f ?  0.6f / scale : xCoord < -0.6f ? -0.6f / scale : player->entity.obj.baseXCoord),
          (yCoord >  0.6f ?  0.6f / scale : yCoord < -0.6f ? -0.6f / scale : player->entity.obj.baseYCoord)
  );

  if(player->entity.xWorldCoord < world->border[0] ||
     player->entity.xWorldCoord > world->border[1] ||
     player->entity.yWorldCoord > world->border[2] ||
     player->entity.yWorldCoord < world->border[3]) playerSendPlayerToSpawn(player, world);

  if(!spacePressed && !dPressed && !aPressed) {
    entityUpdateMovement(&player->entity, 0.0f, 0.0f, world);
    entityChangeTexColumn(&player->entity, STAND_ANIM);
    player->delayToNextTex = player->animationDelay;
    return;
  }


  if(player->delayToNextTex <= 0) {
    entityNextTex(&player->entity);
    player->delayToNextTex = player->animationDelay;
  }
  else player->delayToNextTex--;

  if(spacePressed) {
    if(player->entity.isOnGround) {
      entityJump(&player->entity, world);
      if(player->entity.model.currModelColumn != JUMP_ANIM) 
        entityChangeTexColumn(&player->entity, JUMP_ANIM);
    }
  }  
  if(dPressed) {
    totalHoriMovement += 1.0f;
    if(player->entity.model.currModelColumn != WALK_ANIM) entityChangeTexColumn(&player->entity, WALK_ANIM);
    entitySwitchToSide(&player->entity, RIGHT);
  }
  if(aPressed) {
    totalHoriMovement -= 1.0f;
    if(player->entity.model.currModelColumn != WALK_ANIM) entityChangeTexColumn(&player->entity, WALK_ANIM);
    entitySwitchToSide(&player->entity, LEFT);
  }
  entityUpdateMovement(&player->entity, totalHoriMovement, 0.0f, world);
}


void playerSendPlayerToSpawn(Player* player, World* world) {
  gameObjectSetLocation(&player->entity.obj, world->playerSpawn[0], world->playerSpawn[1]);
  player->entity.xWorldCoord = world->basePlayerSpawn[0];
  player->entity.yWorldCoord = world->basePlayerSpawn[1];
}
