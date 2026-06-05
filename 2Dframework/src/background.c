#include <2Dframework/background.h>
#include <stdio.h>

Background createBackground(const char* image, int colorType, float left, float right, float up, float down, int bgMode) {
  Background background;
  glm_vec2((vec2){0.0f, 0.0f}, background.texOffset);
  background.scale = 1.0f;
  Shader shader = createShader("background/vertex.glsl", "background/fragment.glsl");
  shaderSetVec2(&shader, "texOffset", background.texOffset);
  shaderSetInt(&shader, "tex", 0);
  shaderSetFloat(&shader, "texZoom", background.scale);
  float vertices[16] = {
   -1.0f, 1.0f,   left, up,
    1.0f, 1.0f,   right, up,
   -1.0f,-1.0f,   left, down,
    1.0f,-1.0f,   right, down
  };

  unsigned int indices[6] = {
    0, 1, 2,
    1, 2, 3
  };

  VertexAttrib attrib[2] = {
    {0, 2, 4 * sizeof(float), 0},
    {1, 2, 4 * sizeof(float), 2 * sizeof(float)}
  };

  Mesh mesh = createMesh(vertices, 16, indices, 6, attrib, 2);

  if(bgMode != BG_REPEAT && bgMode != BG_MIRROR) {
    printf("Warning: background %s has an invalid mode, only set to \"BG_REPEAT\" or \"BG_MIRROR\". it has been set to repeat mode."
           , image);
    bgMode = BG_REPEAT;
  }

  Texture texture = createTexture(GL_TEXTURE0);
  textureLoad(&texture, image, bgMode, GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST, colorType);
  
  background.sprite = createSprite(&mesh, &shader, &texture);

  return background;
}

void backgroundDraw(Background* background) {
  spriteDraw(&background->sprite);
}

void backgroundDelete(Background* background) {
  spriteDelete(&background->sprite);
}

void backgroundMove(Background* background, float horizontal, float vertical) {
  background->texOffset[0] += horizontal * background->scale;
  background->texOffset[1] += vertical * background->scale;
  shaderSetVec2(&background->sprite.shader, "texOffset", background->texOffset);
}

void backgroundZoom(Background* background, float level) {
  background->scale += level;
  if(background->scale < 0.0f) background->scale -= level;
  shaderSetFloat(&background->sprite.shader, "texZoom", background->scale);
}

void backgroundSetScale(Background* background, float scale) {
  background->scale = scale;
  shaderSetFloat(&background->sprite.shader, "texZoom", background->scale);
}
