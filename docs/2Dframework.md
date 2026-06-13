# 2Dframework

A 2D side-scrolling game framework built on top of **Randerer** (rendering) and **Database** (persistence). It provides a layered system — `GameObject` → `Entity` → `Player` — along with `Ground`, `Background`, and `World` to compose a complete scrolling game scene with physics, animation, collision, and save/load.

For Randerer's own types (`Randerer`, `Window`, keyboard input with `glfwGetKey`) see **randerer.md**. This document focuses on everything in `<2Dframework/2Dframework.h>`.

---

## Requirements

- Everything required by **Randerer** library
- Everything required by **Database** library
- Texture images placed in the `textures/` folder next to the executable
- Shader files shipped with 2Dframework placed in `shaders/` (handled by CMake automatically)

```cmake
add_subdirectory(2Dframework)           # also pulls in Randerer and Database
target_link_libraries(yourTarget PUBLIC 2Dframework)
```

```c
#include <2Dframework/2Dframework.h>   // includes everything
```

---

## Coordinate System

Everything is in **OpenGL normalised screen space**. The screen centre is `(0, 0)`. Positive X is right, positive Y is up. The visible range before zooming is roughly -1 to +1 on each axis.

`xWorldCoord` / `yWorldCoord` on an `Entity` track the entity's absolute position in the world (unaffected by camera scrolling). The entity's `obj.xCoord` / `obj.yCoord` are its current screen position.

---

## Architecture Overview

```
World
 ├── Background          (scrolling parallax image)
 └── Ground[]            (array of collidable tile sets)
      └── GameObject[]   (individual tiles)

Player
 └── Entity
      └── GameObject     (sprite, transform, texture)
```

`worldDraw` and `playerDraw` are the only draw calls you need in the game loop. Physics, collision, animation, and camera scrolling all happen inside `playerGetUserMovement`.

---

## Complete Minimal Example

```c
#include <2Dframework/2Dframework.h>

int main() {
  // --- Randerer setup (see randerer.md) ---
  Randerer randerer = randererInit("My Game", (int[]){800, 600});
  randererSetAutoFrameResizingKeepRatio(&randerer);

  // --- World ---
  World world = createWorld(
    createBackground("background.png", GL_RGBA,
                     0.0f, 1.0f, 1.0f, 0.0f,  // UV: left, right, up, down
                     BG_REPEAT),
    (Ground[]){
      createGround(64, "tiles.png", GL_RGBA, 0.3f),   // brick-like
      createGround(64, "ice.png",   GL_RGBA, 0.05f),  // slippery
    }, 2,                                  // groundAmount
    (float[2]){0.0f, 1.0f},               // gravity: x=0, y=1
    (float[2]){0.0f, 0.0f},               // player spawn (world coords)
    0.1f,                                 // background parallax factor
    (float[4]){-30.0f, 30.0f, 20.0f, -20.0f} // border: left, right, top, bottom
  );

  // Add some ground tiles
  for (float x = -5.0f; x < 5.0f; x += 0.2f)
    groundAdd(&world.groundArray[0], x, -0.8f, 0.1f, 0.1f, 0.0f);

  // --- Player ---
  // Sprite sheet: 3 columns wide (stand, walk, jump), 3 frames tall
  Player player = createPlayer(
    "player.png", GL_RGBA,
    0.2f,         // animation delay in seconds
    3.0f,         // maxVelocity
    1.0f,         // acceleration
    4.0f,         // jumpPower
    (float[2]){0.3333f, 0.3333f},          // texture cell size (1/cols, 1/rows)
    (TexColumn){0, 1, EN_REPEAT},          // stand: column 0, 1 frame
    (TexColumn){1, 3, EN_REPEAT},          // walk:  column 1, 3 frames
    (TexColumn){2, 1, EN_REPEAT},          // jump:  column 2, 1 frame
    0.0f, 0.0f,   // starting screen position
    0.1f, 0.1f    // width, height
  );

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  playerSendPlayerToSpawn(&player, &world);
  playerSendToLastSavedLocation(&player, &world); // restore save if it exists

  // --- Game loop ---
  while (!randererShouldClose(&randerer)) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    randererUpdateDeltaTime(&randerer);

    worldDraw(&world);
    playerDraw(&player);
    playerGetUserMovement(&player, &randerer, &world);

    // Press S to save position
    if (glfwGetKey(randerer.window.GLFWwindow, GLFW_KEY_S) == GLFW_PRESS)
      playerSaveLocation(&player);

    randererSwapBuffers(&randerer);
  }

  // --- Cleanup ---
  worldDelete(&world);
  playerDelete(&player);
  randererClose(&randerer);
  return 0;
}
```

---

## `Background`

A full-screen scrolling image. The UV coordinates you pass at creation define which portion of the texture is visible initially; `backgroundMove` shifts the UV offset as the world scrolls.

### Constants
| Constant | Value | Meaning |
|---|---|---|
| `BG_REPEAT` | `GL_REPEAT` | Tile the texture |
| `BG_MIRROR` | `GL_MIRRORED_REPEAT` | Mirror-tile the texture |

### `createBackground`
```c
Background createBackground(const char* image, int colorType,
                            float left, float right, float up, float down,
                            int bgMode);
```
Creates a full-screen background sprite.
- `image` — texture file name inside `textures/`
- `colorType` — `GL_RGBA` or `GL_RGB`
- `left`, `right`, `up`, `down` — UV coordinates of the four edges. `(0, 1, 1, 0)` shows the full texture once. Larger values tile it.
- `bgMode` — `BG_REPEAT` or `BG_MIRROR`

### `backgroundDraw`
```c
void backgroundDraw(Background* background);
```
Draws the background. Called automatically by `worldDraw`.

### `backgroundMove`
```c
void backgroundMove(Background* background, float horizontal, float vertical);
```
Shifts the texture UV offset, creating a parallax scroll effect. Called automatically by `worldMove`.

### `backgroundZoom`
```c
void backgroundZoom(Background* background, float level);
```
Adjusts the texture zoom by `level` (additive). Positive = zoom out (more texture visible).

### `backgroundSetScale`
```c
void backgroundSetScale(Background* background, float scale);
```
Sets the texture zoom to an absolute value.

### `backgroundDelete`
```c
void backgroundDelete(Background* background);
```
Frees GPU resources. Called automatically by `worldDelete`.

---

## `Ground`

A set of tile objects that share the same texture and friction. The world can hold multiple `Ground` arrays — one per surface type (brick, ice, mud, etc.).

### `createGround`
```c
Ground createGround(int initialCount, const char texture[MAX_TEX_NAME_SIZE],
                    int colorType, float friction);
```
Allocates a ground set with an initial capacity of `initialCount` tiles. The internal array grows automatically when full.
- `friction` — how quickly horizontal velocity is reduced when the player stands on this surface. `0.0` = no friction (frictionless), `1.0` = instant stop.

### `groundAdd`
```c
void groundAdd(Ground* ground, float xCoord, float yCoord,
               float width, float height, float rotation);
```
Adds one tile at the given position, size, and rotation. This is the main function you use to build a level. The tile array is enlarged automatically if needed.

```c
// Add a row of tiles along y = -0.8
for (float x = -5.0f; x < 5.0f; x += 0.2f)
  groundAdd(&world.groundArray[0], x, -0.8f, 0.1f, 0.1f, 0.0f);
```

### `groundEnlarge`
```c
void groundEnlarge(Ground* ground, int amount);
```
Manually grows the tile array by `amount` slots. `groundAdd` does this automatically, so you only need this if you want to pre-allocate capacity in advance.

### `groundDeleteInSlot`
```c
void groundDeleteInSlot(Ground* ground, int slot);
```
Frees the GPU resources for the tile at index `slot` without removing the slot itself from the array.

### `groundDraw`
```c
void groundDraw(Ground* ground);
```
Draws all tiles. Called automatically by `worldDraw`.

### `groundMove`
```c
void groundMove(Ground* ground, float horizontal, float vertical);
```
Shifts all tiles in this ground set. Called automatically by `worldMove` (camera scroll). You generally do not call this directly.

### `groundZoom` / `groundSetScale`
```c
void groundZoom(Ground* ground, float level);
void groundSetScale(Ground* ground, float scale);
```
Adjust the zoom of all tiles in the set (additive delta vs. absolute value). Called automatically by `worldZoom` / `worldSetScale`.

### `groundCheckCollision`
```c
int groundCheckCollision(Ground* ground, GameObject* object);
```
Returns `1` if `object` overlaps any tile in this ground set, `0` otherwise.

### `groundDelete`
```c
void groundDelete(Ground* ground);
```
Frees all tile GPU resources and the tile array. Called automatically by `worldDelete`.

---

## `World`

Bundles a background and all ground sets into one object. Also stores gravity, the player spawn point, and the kill-zone border.

### `createWorld`
```c
World createWorld(Background bg, Ground* groundArray, int groundAmount,
                  float gravityLevel[2], float playerSpawn[2],
                  float bgMovementWithGround, float border[4]);
```
- `bg` — Background object for the world
- `groundArray` — a temporary array of `Ground` values; they are copied internally.
- `groundAmount` — number of grounds in the array.
- `gravityLevel` — `{x, y}` gravity applied to entities each frame. Typically `{0.0f, 1.0f}` (downward).
- `playerSpawn` — world coordinates where `playerSendPlayerToSpawn` places the player.
- `bgMovementWithGround` — parallax factor for the background when the world scrolls. `0.0` = background fixed, `1.0` = background moves with the world.
- `border` — `{left, right, top, bottom}` in world coordinates. When the player's world position leaves this rectangle, `playerGetUserMovement` automatically sends them back to spawn (and then to the last saved location).

### `worldDraw`
```c
void worldDraw(World* world);
```
Draws the background then all ground tiles. Call once per frame before `playerDraw`.

### `worldMove`
```c
void worldMove(World* world, float horizontal, float vertical);
```
Scrolls all ground tiles and the background by the given amount (camera movement). Called automatically by `playerGetUserMovement`. If you need to manually reposition the camera (for example after a teleport), call this with the negative of the accumulated offset — see the camera-reset pattern below.

### `worldZoom` / `worldSetScale`
```c
void worldZoom(World* world, float level);
void worldSetScale(World* world, float scale);
```
Apply zoom or an absolute scale to the background and all ground tiles. Use together with `entityZoom` on the player so everything scales uniformly:
```c
worldZoom(&world, 0.05f);
entityZoom(&player.entity, 0.05f);
```

### `worldCheckCollision`
```c
int worldCheckCollision(World* world, GameObject* object);
```
Checks `object` against every ground set. Returns the **index** of the first ground that collides, or `-1` if none. Used internally by `entityUpdateMovement`; you can also query it to detect which surface type the player is touching.

### `worldDelete`
```c
void worldDelete(World* world);
```
Frees the background, all ground tiles, and the internal ground array.

---

## `GameObject`

The lowest-level visible object: a textured square on screen with a position, size, and rotation. `Entity` and `Ground` build on top of this.

### Key fields
```c
float xCoord, yCoord;       // current screen position
float width, height;        // current scaled size
float rotation;             // degrees
float scale;                // uniform scale multiplier
float baseXCoord, baseYCoord; // position before scale is applied
float baseWidth, baseHeight;  // size before scale is applied
```

### `createGameObject`
```c
GameObject createGameObject(const char* image, int colorType, int texWrap,
                            Mesh mesh, float xCoord, float yCoord,
                            float width, float height, float rotation);
```
Creates a textured game object. For entity meshes you usually pass `createEntityMesh(modelSize)` as the mesh. Ground tiles use `GO_SQUARE`.

### `gameObjectDraw`
```c
void gameObjectDraw(GameObject* object);
```
Draws the object using its sprite.

### `gameObjectMove`
```c
void gameObjectMove(GameObject* object, float horizontal, float vertical);
```
Moves the object by a delta in screen space, updating both base and scaled coordinates.

### `gameObjectSetLocation`
```c
void gameObjectSetLocation(GameObject* object, float xCoord, float yCoord);
```
Moves the object to an absolute screen position.

### `gameObjectResize` / `gameObjectSetSize`
```c
void gameObjectResize(GameObject* object, float horizontal, float vertical);
void gameObjectSetSize(GameObject* object, float height, float width);
```
Adjust the object's size (delta vs. absolute).

### `gameObjectRotate`
```c
void gameObjectRotate(GameObject* object, float rotation);
```
Rotates the object by `rotation` degrees (additive).

### `gameObjectZoom` / `gameObjectSetScale`
```c
void gameObjectZoom(GameObject* object, float level);
void gameObjectSetScale(GameObject* object, float scale);
```
Adjust the uniform scale of the object (additive delta vs. absolute).

### `gameObjectMoveTex`
```c
void gameObjectMoveTex(GameObject* object, float horizontal, float vertical);
```
Scrolls the texture UV offset of the object. Useful for animating tiled surfaces.

### `gameObjectChangeTexFile`
```c
void gameObjectChangeTexFile(GameObject* object, const char* image, int colorType);
```
Swaps the object's texture for a different image file at runtime.

### `gameObjectCheckCollision`
```c
int gameObjectCheckCollision(GameObject* object1, GameObject* object2);
```
AABB collision test. Returns `1` if the two objects overlap, `0` otherwise.

### `gameObjectDelete`
```c
void gameObjectDelete(GameObject* object);
```
Frees the object's sprite (mesh, shader, texture) from the GPU.

---

## `Entity`

An animated, physics-driven game object. Adds velocity, gravity, collision response, jump mechanics, and sprite-sheet animation on top of `GameObject`.

### Key fields
```c
float xWorldCoord, yWorldCoord; // absolute world position (unaffected by camera)
float currHoriVelocity;         // current horizontal velocity
float currVertVelocity;         // current vertical velocity
int   currStandedOnGround;      // index of ground being stood on, or -1
float jumpPower;                // initial vertical impulse on jump
float maxVelocity;              // horizontal velocity cap
float accelaration;             // horizontal acceleration per movement input
```

### Animation — `TexColumn` and `ModelAttrib`

The sprite sheet is divided into columns (animations) and rows (frames). `TexColumn` describes one animation:
```c
typedef struct {
  int column;      // which column of the sprite sheet
  int count;       // how many frames in this animation
  int actionAtEnd; // EN_REPEAT or EN_MIRROR
} TexColumn;
```
`modelSize[2]` in `ModelAttrib` is the UV size of one cell: `{1.0f / numColumns, 1.0f / numRows}`.

### Constants
| Constant | Meaning |
|---|---|
| `EN_USE_COLLISION` | Entity participates in collision |
| `EN_IGNORE_COLLISION` | Entity ignores collision (ghost) |
| `EN_REPEAT` | Animation loops back to frame 1 at the end |
| `EN_MIRROR` | Animation reverses at the end |

### `createEntity`
```c
Entity createEntity(const char* image, int colorType, ModelAttrib* model,
                    int ignoreCollision, float accelaration, float maxVelocity,
                    float jumpPower, float xCoord, float yCoord,
                    float width, float height);
```
Creates a physics entity. Internally creates the `GameObject` and configures the collision step size.

### `entityUpdateMovement`
```c
void entityUpdateMovement(Entity* entity, float horiMovement, float vertMovement,
                          Randerer* randerer, World* world);
```
The physics tick. Pass `horiMovement` as `-1.0` (left), `0.0` (no input), or `+1.0` (right). `vertMovement` is usually `0.0`. This function: applies acceleration and friction, integrates velocity via `randerer->deltaTime`, moves the entity, and resolves collisions with the world.

### `entityJumpNOUPDATE`
```c
void entityJumpNOUPDATE(Entity* entity, Randerer* randerer);
```
Sets the jump impulse without calling `entityUpdateMovement`. Use this when you want the jump to be processed on the same tick as movement, which is what `playerGetUserMovement` does.

### `entityJump`
```c
void entityJump(Entity* entity, Randerer* randerer, World* world);
```
Sets the jump impulse and immediately calls `entityUpdateMovement`. Useful for entities you control directly outside of a Player.

### `entityChangeTexColumn`
```c
void entityChangeTexColumn(Entity* entity, int column);
```
Switches to a different animation column and resets to frame 1.

### `entityNextTex`
```c
void entityNextTex(Entity* entity);
```
Advances to the next animation frame. At the end of the animation applies `EN_REPEAT` or `EN_MIRROR`.

### `entitySwitchToSide`
```c
void entitySwitchToSide(Entity* entity, Direction side);
```
Flips the sprite left (`LEFT`) or right (`RIGHT`) by adjusting the UV offset.

### `entityZoom` / `entitySetScale`
```c
void entityZoom(Entity* entity, float level);
void entitySetScale(Entity* entity, float scale);
```
Zoom or set the scale of the entity's game object. Also recalculates the collision step size.

### `entityClearCache`
```c
void entityClearCache(Entity* entity);
```
Resets velocity, ground-contact state, facing direction, and animation to their defaults.

### `entityDraw` / `entityDelete`
```c
void entityDraw(Entity* entity);
void entityDelete(Entity* entity);
```
Draw or free the entity.

### `createEntityMesh`
```c
Mesh createEntityMesh(float texCoord[2]);
```
Creates the vertex mesh for an entity given the UV cell size `{1.0/numColumns, 1.0/numRows}`. Used internally by `createEntity`.

---

## `Player`

A ready-to-use player entity with three built-in animations (stand, walk, jump), keyboard input handling, camera scrolling, a save/load system, and a border respawn system. Built on top of `Entity` and `Database`.

### Animation indices
```c
#define STAND_ANIM 0
#define WALK_ANIM  1
#define JUMP_ANIM  2
```

### Default controls
| Key | Action |
|---|---|
| `A` | Move left |
| `D` | Move right |
| `Space` | Jump (only when on the ground) |

### `createPlayer`
```c
Player createPlayer(const char* image, int colorType, float animationDelay,
                    float maxVelocity, float accelaration, float jumpPower,
                    float modelSize[2],
                    TexColumn standAnim, TexColumn walkAnim, TexColumn jumpAnim,
                    float xCoord, float yCoord, float width, float height);
```
Creates a player with three animations on one sprite sheet. `animationDelay` is the number of frames between animation steps (higher = slower animation). The player save file is always `player.dat` (BINARY).

**Sprite-sheet layout example** — 3 columns, 3 rows:
```
Column 0 (stand): row 0
Column 1 (walk):  rows 0–2
Column 2 (jump):  row 0
```
```c
(float[2]){1.0f/3, 1.0f/3}      // modelSize: one cell = 1/3 of sheet
(TexColumn){0, 1, EN_REPEAT}    // stand: col 0, 1 frame
(TexColumn){1, 3, EN_REPEAT}    // walk:  col 1, 3 frames, loops
(TexColumn){2, 1, EN_REPEAT}    // jump:  col 2, 1 frame
```

### `playerGetUserMovement`
```c
void playerGetUserMovement(Player* player, Randerer* randerer, World* world);
```
The all-in-one player tick. Each frame it:
1. Scrolls the camera to keep the player within ±0.6 screen units of centre.
2. Checks the world border; if the player is outside, calls `playerSendPlayerToSpawn` then `playerSendToLastSavedLocation`.
3. Reads keyboard input (A / D / Space).
4. Updates the animation.
5. Calls `entityUpdateMovement`.

Call once per frame inside the game loop.

### `playerSendPlayerToSpawn`
```c
void playerSendPlayerToSpawn(Player* player, World* world);
```
Instantly moves the player to the world's spawn position (the `playerSpawn` values from `createWorld`). Also called automatically when the player leaves the border.

### `playerSaveLocation`
```c
void playerSaveLocation(Player* player);
```
Writes `xWorldCoord` and `yWorldCoord` to `player.dat`. Use this when the player hits a checkpoint or presses a save key:
```c
if (glfwGetKey(randerer.window.GLFWwindow, GLFW_KEY_S) == GLFW_PRESS)
  playerSaveLocation(&player);
```

### `playerSendToLastSavedLocation`
```c
void playerSendToLastSavedLocation(Player* player, World* world);
```
Reads the saved position from `player.dat` and teleports the player there, correctly resetting the camera offset so the world is in the right visual position. If no save exists this is a no-op. Also called automatically when the player leaves the border.

### `playerDeleteSavedLocation`
```c
void playerDeleteSavedLocation(Player* player);
```
Removes the saved position entries from `player.dat`.

### `playerDraw`
```c
void playerDraw(Player* player);
```
Draws the player sprite.

### `playerDelete`
```c
void playerDelete(Player* player);
```
Frees the player's entity GPU resources.

---

## Zoom / Camera Scale

You can let the player zoom in and out at runtime. Always zoom both the world and the entity together:

```c
void getZoomControl(Player* player, World* world, Randerer* randerer) {
  if (glfwGetKey(randerer->window.GLFWwindow, GLFW_KEY_MINUS) == GLFW_PRESS) {
    worldZoom(world, -0.05f);
    entityZoom(&player->entity, -0.05f);
  }
  if (glfwGetKey(randerer->window.GLFWwindow, GLFW_KEY_EQUAL) == GLFW_PRESS) {
    worldZoom(world, 0.05f);
    entityZoom(&player->entity, 0.05f);
  }
}
```

---

## Camera Reset After Teleport

`playerSendToLastSavedLocation` already handles this internally. If you ever teleport the player manually (setting `xWorldCoord` / `yWorldCoord` directly) you must also reset the camera, otherwise the visual position will be wrong:

```c
// Reset accumulated camera offset before changing world position
worldMove(&world,
    player.entity.xWorldCoord - player.entity.obj.baseXCoord,
    player.entity.yWorldCoord - player.entity.obj.baseYCoord);

// Now set the new position
player.entity.xWorldCoord = targetX;
player.entity.yWorldCoord = targetY;
gameObjectSetLocation(&player.entity.obj, targetX, targetY);
```

---

## Multiple Ground Types

Use separate `Ground` entries for surfaces with different friction values. `worldCheckCollision` returns which ground index was hit, so you can query surface type:

```c
Ground grounds[3] = {
  createGround(128, "brick.png", GL_RGBA, 0.30f),  // [0] normal
  createGround(64,  "ice.png",   GL_RGBA, 0.05f),  // [1] slippery
  createGround(64,  "mud.png",   GL_RGBA, 0.60f),  // [2] sticky
};
```

```c
int surface = worldCheckCollision(&world, &player.entity.obj);
if (surface == 1) { /* player is on ice */ }
```
