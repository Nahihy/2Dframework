#include <2Dframework/entity.h>
#include <stdlib.h>

Mesh createEntityMesh(float texCoord[2]) {
  float vertices[16] = {
   -1.0f, 1.0f,   0.0f,        texCoord[1],
    1.0f, 1.0f,   texCoord[0], texCoord[1],
   -1.0f,-1.0f,   0.0f,        0.0f,
    1.0f,-1.0f,   texCoord[0], 0.0f
  };
  unsigned int indices[6] = {
    0, 1, 2,
    1, 2, 3
  };

  VertexAttrib attrib[2] = {
    {0, 2, 4 * sizeof(float), 0},
    {1, 2, 4 * sizeof(float), 2 * sizeof(float)}
  };

  return createMesh(vertices, 16, indices, 6, attrib, 2);
}

Entity createEntity(const char* image, int colorType, ModelAttrib* model, int ignoreCollision, float accelaration,
                    float maxVelocity, float jumpPower, float xCoord, float yCoord, float width, float height) {
  if(ignoreCollision != EN_IGNORE_COLLISION && ignoreCollision != EN_USE_COLLISION) {
    printf("Warning: invalid collision value in %s entity. only set to \"EN_USE_COLLISION\" or \"EN_IGNORE_COLLISION\", it has been set to ignore", image);
    ignoreCollision = EN_IGNORE_COLLISION;
  }
  Entity entity;
  entity.ignoreCollision = ignoreCollision;
  entity.model = *model;
  entity.accelaration = accelaration;
  entity.maxVelocity = maxVelocity;
  entity.currHoriVelocity = 0.0f;
  entity.currVertVelocity = 0.0f;
  entity.model.side = RIGHT;
  entity.currStandedOnGround = -1;
  entity.currJumpAccel = 0.0f;
  entity.jumpPower = jumpPower;
  entity.xWorldCoord = xCoord;
  entity.yWorldCoord = yCoord;

  entity.obj = createGameObject(image, colorType, GL_MIRRORED_REPEAT, createEntityMesh(entity.model.modelsize),
                                xCoord, yCoord, width, height, 0.0f);
  entityUpdateTex(&entity);
  entity.baseCollisionStep = (entity.obj.width + entity.obj.height) / 20;
  entity.collisionStep = entity.baseCollisionStep;
  return entity;
}

void entityUpdateMovement(Entity* entity, float horiMovement, float vertMovement, Randerer* randerer, World* world) {

  float horiAccel = entity->accelaration * horiMovement;

  entity->currHoriVelocity += horiAccel * randerer->deltaTime;

  if (entity->currHoriVelocity > entity->maxVelocity) entity->currHoriVelocity = entity->maxVelocity;
  else if (entity->currHoriVelocity < -entity->maxVelocity) entity->currHoriVelocity = -entity->maxVelocity;

  if(entity->currStandedOnGround != -1)
    entity->currHoriVelocity -= world->groundArray[entity->currStandedOnGround].friction * entity->currHoriVelocity * randerer->deltaTime * 60.0f;


  entity->currHoriVelocity -= world->gravityLevel[0] * randerer->deltaTime;

  Direction direction;
  if(entity->currHoriVelocity > 0.0f) direction = RIGHT;
  else direction = LEFT;
  entity->currHoriVelocity -= world->airResistence * entity->currHoriVelocity * randerer->deltaTime * 60.0f;  
  if(direction * entity->currHoriVelocity < 0.0f) entity->currHoriVelocity = 0.0f;

  entity->xWorldCoord += entity->currHoriVelocity * randerer->deltaTime;
  gameObjectMove(&entity->obj, entity->currHoriVelocity * randerer->deltaTime, 0);

  if (entity->ignoreCollision == EN_USE_COLLISION && worldCheckCollision(world, &entity->obj) != -1) {
    entity->xWorldCoord -= entity->currHoriVelocity * randerer->deltaTime;
    gameObjectMove(&entity->obj, -(entity->currHoriVelocity * randerer->deltaTime), 0);
    float dir = (entity->currHoriVelocity > 0.0f) ? entity->collisionStep : -entity->collisionStep;
    entity->currHoriVelocity = 0.0f;
    while (worldCheckCollision(world, &entity->obj) == -1) {
      entity->xWorldCoord += dir;
      gameObjectMove(&entity->obj, dir, 0);
    }
    entity->xWorldCoord -= dir;
    gameObjectMove(&entity->obj, -dir, 0);
  }

  float vertAccel = entity->accelaration * vertMovement + entity->currJumpAccel;
  entity->currJumpAccel *= powf(1.0f / 1.5f, randerer->deltaTime * 60.0f);

  entity->currVertVelocity += vertAccel * randerer->deltaTime;

  entity->currVertVelocity -= world->gravityLevel[1] * randerer->deltaTime;

  if(entity->currVertVelocity > 0.0f) direction = RIGHT;
  else direction = LEFT;
  entity->currVertVelocity -= world->airResistence * entity->currVertVelocity * randerer->deltaTime * 60.0f;
  if(entity->currVertVelocity * direction < 0.0f) entity->currVertVelocity = 0.0f;

  entity->yWorldCoord += entity->currVertVelocity * randerer->deltaTime;
  gameObjectMove(&entity->obj, 0, entity->currVertVelocity * randerer->deltaTime);

  if (entity->ignoreCollision == EN_USE_COLLISION) {
    entity->currStandedOnGround = worldCheckCollision(world, &entity->obj);
    if(entity->currStandedOnGround != -1) {
      entity->yWorldCoord -= entity->currVertVelocity * randerer->deltaTime;
      gameObjectMove(&entity->obj, 0, -(entity->currVertVelocity * randerer->deltaTime));
      if (entity->currVertVelocity > 0.0f)
        entity->currStandedOnGround = -1;
      float dir = (entity->currVertVelocity > 0.0f) ? entity->collisionStep : -entity->collisionStep;
      entity->currVertVelocity = 0.0f;
      while (worldCheckCollision(world, &entity->obj) == -1) {
        entity->yWorldCoord += dir;
        gameObjectMove(&entity->obj, 0, dir);
      }
      entity->yWorldCoord -= dir;
      gameObjectMove(&entity->obj, 0, -dir);
    }
  } else entity->currStandedOnGround = -1;
}

void entityDelete(Entity* entity) {
  gameObjectDelete(&entity->obj);
}

void entityDraw(Entity* entity) {
  gameObjectDraw(&entity->obj);
}

void entityMove(Entity* entity, float horizontal, float vertical) {
  gameObjectMove(&entity->obj, horizontal, vertical);
}

void entityResize(Entity* entity, float horizontal, float vertical) {
  gameObjectResize(&entity->obj, horizontal, vertical);
  entity->baseCollisionStep = (entity->obj.baseWidth * entity->obj.baseHeight) / 20;
  entity->collisionStep = entity->baseCollisionStep * entity->obj.scale;
}

void entitySetSize(Entity* entity, float height, float width) {
  gameObjectSetSize(&entity->obj, height, width);
  entity->baseCollisionStep = (entity->obj.baseWidth * entity->obj.baseHeight) / 20;
  entity->collisionStep = entity->baseCollisionStep * entity->obj.scale;
}

void entityZoom(Entity* entity, float level) {
  gameObjectZoom(&entity->obj, level);
  entity->collisionStep = entity->baseCollisionStep * entity->obj.scale;
}

void entitySetScale(Entity* entity, float scale) {
  gameObjectSetScale(&entity->obj, scale);
  entity->collisionStep = entity->baseCollisionStep * entity->obj.scale;
}

void entityNextTex(Entity* entity) {
  entity->model.currModel += entity->model.nextTexMov;
  TexColumn* currColumn = &(entity->model.modelColumns[entity->model.currModelColumn]);
  if(currColumn->count < entity->model.currModel)
    if(currColumn->actionAtEnd == EN_REPEAT) entity->model.currModel = 1;
    else if(currColumn->actionAtEnd == EN_MIRROR) {
      entity->model.currModel -= 2 * entity->model.nextTexMov;
      entity->model.nextTexMov = -entity->model.nextTexMov;
    }
  if(entity->model.currModel < 1) {
    entity->model.nextTexMov = abs(entity->model.nextTexMov);
    entity->model.currModel = 1;
  }
  entityUpdateTex(entity);
}

void entityChangeTexColumn(Entity* entity, int column) {
  entity->model.currModel = 1;
  entity->model.currModelColumn = column;
  entityUpdateTex(entity);
}

void entityUpdateTex(Entity* entity) {
  int column = entity->model.modelColumns[entity->model.currModelColumn].column;
  float width = entity->model.modelsize[0];

  if (entity->model.side == LEFT) {
    entity->obj.horiTexOffset = -(column + 1) * width;
  } else {
    entity->obj.horiTexOffset = column * width;
  }

  entity->obj.vertTexOffset = 1.0f - (entity->model.currModel * entity->model.modelsize[1]);
  gameObjectUpdateTex(&entity->obj);
}

void entitySwitchToSide(Entity* entity, Direction side) {
  entity->model.side = side;
  entityUpdateTex(entity);
}

void entityJumpNOUPDATE(Entity* entity, Randerer* randerer) {
  entity->currJumpAccel = entity->jumpPower;
}

void entityJump(Entity* entity, Randerer* randerer, World* world) {
  entity->currJumpAccel = entity->jumpPower;
  entityUpdateMovement(entity, 0.0f, 0.0f, randerer, world);
}

void entityClearCache(Entity* entity) {
  entity->currHoriVelocity = 0.0f;
  entity->currVertVelocity = 0.0f;
  entity->currStandedOnGround = -1;
  entitySwitchToSide(entity, RIGHT);
  entityChangeTexColumn(entity, 0);
}
