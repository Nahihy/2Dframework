#include <2Dframework/entity.h>
#include <stdio.h>

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
  entity.isOnGround = 0;
  entity.currJumpAccel = 0.0f;
  entity.jumpPower = jumpPower;
  entity.obj = createGameObject(image, colorType, GL_MIRRORED_REPEAT, createEntityMesh(entity.model.modelsize),
                                xCoord, yCoord, width, height, 0.0f);
  entity.collisionStep = 0.1f;
  entityUpdateTex(&entity);
  return entity;
}

void entityUpdateMovement(Entity* entity, float horiMovement, float vertMovement, World* world) {

  float totalHoriMovement = entity->accelaration * horiMovement - (world->gravityLevel[0] * 0.01f);

  if (!horiMovement) {
    if (entity->currHoriVelocity > 0.0f) {
      totalHoriMovement -= entity->accelaration / 2.0f;
      if (entity->currHoriVelocity + totalHoriMovement < 0.0f)
        totalHoriMovement = -entity->currHoriVelocity;
    } else if (entity->currHoriVelocity < 0.0f) {
      totalHoriMovement += entity->accelaration / 2.0f;
      if (entity->currHoriVelocity + totalHoriMovement > 0.0f)
        totalHoriMovement = -entity->currHoriVelocity;
    }
  }

  if(entity->currHoriVelocity > entity->maxVelocity) totalHoriMovement -= entity->accelaration;
  else if(entity->currHoriVelocity < -entity->maxVelocity) totalHoriMovement += entity->accelaration;

  gameObjectMove(&entity->obj, entity->currHoriVelocity + totalHoriMovement, 0);
  if (entity->ignoreCollision == EN_USE_COLLISION && groundCheckCollision(&world->ground, &entity->obj)) {
    gameObjectMove(&entity->obj, -(entity->currHoriVelocity + totalHoriMovement), 0);
    entity->currHoriVelocity = 0.0f;
    float dir = (totalHoriMovement > 0.0f) ? entity->collisionStep : -entity->collisionStep;
    totalHoriMovement = 0.0f;
    while (!groundCheckCollision(&world->ground, &entity->obj)) {
      gameObjectMove(&entity->obj, dir, 0);
    }
    gameObjectMove(&entity->obj, -dir, 0);
  }  


  float totalVertMovement = entity->accelaration * vertMovement - (world->gravityLevel[1] * 0.01f) + entity->currJumpAccel;
  entity->currJumpAccel /= 1.5f;

  if (!vertMovement) {
    if (entity->currVertVelocity > 0.0f) {
      totalVertMovement -= entity->accelaration / 2.0f;
      if (entity->currVertVelocity + totalVertMovement < 0.0f)
        totalVertMovement = -entity->currVertVelocity;
    } else if (entity->currVertVelocity < 0.0f) {
      totalVertMovement += entity->accelaration / 2.0f;
      if (entity->currVertVelocity + totalVertMovement > 0.0f)
        totalVertMovement = -entity->currVertVelocity;
    }
  }

  if(entity->currVertVelocity > entity->maxVelocity) totalVertMovement -= entity->accelaration;
  else if(entity->currVertVelocity < -entity->maxVelocity) totalVertMovement += entity->accelaration;

  gameObjectMove(&entity->obj, 0, entity->currVertVelocity + totalVertMovement);
  if (entity->ignoreCollision == EN_USE_COLLISION && groundCheckCollision(&world->ground, &entity->obj)) {
    gameObjectMove(&entity->obj, 0, -(entity->currVertVelocity + totalVertMovement));
    entity->currVertVelocity = 0.0f;
    if (entity->currVertVelocity + totalVertMovement < 0.0f)
      entity->isOnGround = 1;
    float dir = (totalVertMovement > 0.0f) ? entity->collisionStep : -entity->collisionStep;
    totalVertMovement = 0.0f;
    while (!groundCheckCollision(&world->ground, &entity->obj)) {
      gameObjectMove(&entity->obj, 0, dir);
    }
    gameObjectMove(&entity->obj, 0, -dir);
  }  else entity->isOnGround = 0;


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
}

void entityZoom(Entity* entity, float mult) {
  gameObjectSetSize(&entity->obj, entity->obj.height * mult, entity->obj.width * mult);
  gameObjectSetLocation(&entity->obj, entity->obj.xCoord * mult, entity->obj.yCoord * mult);
  entity->maxVelocity *= mult;
  entity->accelaration *= mult;
  entity->currJumpAccel *= mult;
  entity->jumpPower *= mult;
  entity->collisionStep *= mult;
}
void entityNextTex(Entity* entity) {
  entity->model.currModel++;
  TexColumn* currColumn = &(entity->model.modelColumns[entity->model.currModelColumn]);
  if(currColumn->count < entity->model.currModel)
    if(currColumn->actionAtEnd == EN_REPEAT) entity->model.currModel = 1;
    else if(currColumn->actionAtEnd == EN_MIRROR) entity->model.currModel -= 2;
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

void entityJump(Entity* entity, World* world) {
  entity->currJumpAccel = entity->jumpPower;
  entity->isOnGround = 0;
  entityUpdateMovement(entity, 0.0f, 0.0f, world);
}
