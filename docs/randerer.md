# Randerer Library

A thin OpenGL 2D rendering library. It initialises a window, manages an OpenGL context, tracks frame time (delta time), and provides building-block types — `Mesh`, `Shader`, `Texture`, and `Sprite` — that you compose to draw things on screen.

In practice you will rarely use the lower-level types directly when working inside 2Dframework; they are used internally by `GameObject` and the rest of the framework. The important surface you interact with is the `Randerer` struct and its functions.

---

## Requirements

- OpenGL 3.3 Core Profile capable GPU / driver
- CMake 3.14+
- Standard C compiler
- The following bundled dependencies (added via `add_subdirectory` in the CMakeLists. they are already included):
  - **GLFW** — window creation and input
  - **glad** — OpenGL function loader
  - **cglm** — math (matrices, vectors)
  - **stbi** — image loading

```cmake
add_subdirectory(path/to/randerer)
target_link_libraries(yourTarget PUBLIC randerer)
```

Include with:
```c
#include <randerer/randerer.h>
```
This pulls in `mesh.h`, `shader.h`, `window.h`, and `texture.h` automatically.

---

## Directory Layout Expected at Runtime

By default the build system copies these folders next to the executable:

| Folder | Contents |
|---|---|
| `shaders/` | GLSL shader files referenced by path |
| `textures/` | Image files referenced by path |

If you use the `EMBED_SHADERS` CMake option, shader source is compiled directly into the binary and the `shaders/` folder is not needed at runtime.

The dir they are copied from is set by default to 'shaders' and 'textures' in the project root, can be changed by setting SHADER_DIR/TEXTURE_DIR in the cmake file. 

---

## Core Types

### `Randerer`
```c
typedef struct {
  float  lastFrame;
  float  deltaTime;
  Window window;
} Randerer;
```
The central object you create once and pass everywhere. `deltaTime` is updated each frame and tells you how many seconds elapsed since the last frame — use it to make movement frame-rate independent.

### `Window`
```c
typedef struct {
  GLFWwindow* GLFWwindow;
  const char* windowTitle;
  int         windowDimensions[2];
} Window;
```
Wraps a GLFW window. Accessible as `randerer.window`. You need `randerer.window.GLFWwindow` to poll keyboard input with `glfwGetKey`.

### `Sprite`
```c
typedef struct {
  Shader  shader;
  Mesh    mesh;
  Texture texture;
  int     isTextured;
} Sprite;
```
Groups a mesh, a shader program, and an optional texture into one drawable unit. Created with `createSprite` and drawn with `spriteDraw`.

### `Mesh`
Holds GPU buffers (VAO, VBO, EBO) and the vertex/index data needed to describe a shape.

### `Shader`
Wraps an OpenGL shader program (vertex + fragment). Provides helpers to upload uniforms.

### `Texture`
Wraps an OpenGL texture object and its texture unit.

---

## Basic Usage

```c
#include <randerer/randerer.h>

int main() {
  // 1. Initialise — creates window + OpenGL context
  Randerer randerer = randererInit("My Window", (int[]){800, 600});

  // 2. Optional: resize the viewport automatically when the window is resized,
  //    preserving the original aspect ratio
  randererSetAutoFrameResizingKeepRatio(&randerer);

  // 3. Game loop
  while (!randererShouldClose(&randerer)) {
    // Update delta time at the start of every frame
    randererUpdateDeltaTime(&randerer);

    // --- clear screen ---
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // --- draw things here ---

    // Present the frame and poll events
    randererSwapBuffers(&randerer);
  }

  // 4. Clean up
  randererClose(&randerer);
  return 0;
}
```

### Drawing a textured sprite

```c
// Build the pieces
Mesh    mesh    = createSquareMeshTemp();
Shader  shader  = createShader("gameObject/vertex.glsl", "gameObject/fragment.glsl");
Texture texture = createTexture(GL_TEXTURE1);
textureLoad(&texture, "myImage.png", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST, GL_RGBA);

// Combine into a sprite
Sprite sprite = createSprite(&mesh, &shader, &texture);

// In the game loop:
spriteDraw(&sprite);

// On shutdown:
spriteDelete(&sprite);
```

### Reading keyboard input

The raw GLFW window pointer is available through the `Randerer`:
```c
if (glfwGetKey(randerer.window.GLFWwindow, GLFW_KEY_SPACE) == GLFW_PRESS) {
  // space bar is held
}
```

---

## Function Reference

### `Randerer` — Lifecycle & Frame Management

#### `randererInit`
```c
Randerer randererInit(const char* windowTitle, int windowDimensions[2]);
```
Initialises GLFW, creates a window of the given pixel size, loads OpenGL via glad, and sets up the initial viewport. Returns the fully initialised `Randerer`. Must be called before anything else.

#### `randererClose`
```c
void randererClose(Randerer* randerer);
```
Destroys the window and terminates GLFW. Call this once at the end of the program.

#### `randererUpdateDeltaTime`
```c
void randererUpdateDeltaTime(Randerer* randerer);
```
Recalculates `randerer.deltaTime` as the time in seconds since the last call. Call once at the top of each frame so that physics and movement scale correctly regardless of frame rate.

#### `randererShouldClose`
```c
int randererShouldClose(Randerer* randerer);
```
Returns non-zero when the user has closed the window or your code has requested termination. Use as the game-loop condition.

#### `randererSwapBuffers`
```c
void randererSwapBuffers(Randerer* randerer);
```
Swaps the front and back buffers (presents the rendered frame) and polls window events. Call at the end of each frame.

---

### `Randerer` — Window Resize Behaviour

#### `randererSetAutoFrameResizing`
```c
void randererSetAutoFrameResizing(Randerer* randerer);
```
Registers a callback so the OpenGL viewport always fills the entire window when it is resized.

#### `randererSetAutoFrameResizingKeepRatio`
```c
void randererSetAutoFrameResizingKeepRatio(Randerer* randerer);
```
Like the above, but adds letterboxing / pillarboxing so the original aspect ratio is preserved. The black bars appear automatically; nothing renders into them.

---

### `Sprite`

#### `createSprite`
```c
Sprite createSprite(Mesh* mesh, Shader* shader, Texture* texture);
```
Combines a mesh, shader, and optional texture into a `Sprite`. Pass `NULL` for `texture` to create an untextured sprite.

#### `spriteDraw`
```c
void spriteDraw(Sprite* sprite);
```
Binds the texture (if any), activates the shader, and issues the draw call for the mesh.

#### `spriteDelete`
```c
void spriteDelete(Sprite* sprite);
```
Frees GPU resources for the mesh, shader, and texture.

---

### `Mesh`

#### `createMesh`
```c
Mesh createMesh(float* vertices, unsigned int vertexCount,
                unsigned int* indices, unsigned int indexCount,
                VertexAttrib* attrib, unsigned int attribCount);
```
Uploads vertex and index data to the GPU and configures vertex attributes. Returns a ready-to-draw `Mesh`.

#### `createSquareMeshTemp`
```c
Mesh createSquareMeshTemp();
```
Convenience function that returns a unit square mesh (two triangles covering -1 to +1 in both axes).

#### `createTriangleMeshTemp`
```c
Mesh createTriangleMeshTemp();
```
Returns a simple triangle mesh for testing.

#### `meshChangeVerticies`
```c
void meshChangeVerticies(Mesh* mesh, float* vertices);
```
Re-uploads a new set of vertex data to the GPU for an existing mesh.

#### `meshDraw`
```c
void meshDraw(Mesh* mesh);
```
Issues the OpenGL draw call for this mesh.

#### `meshDelete`
```c
void meshDelete(Mesh* mesh);
```
Deletes the VAO, VBO, and EBO from the GPU and frees the CPU-side arrays.

---

### `Shader`

#### `createShader`
```c
Shader createShader(const char* vertexPath, const char* fragmentPath);
```
Loads vertex and fragment GLSL source from files (relative to the `shaders/` directory), compiles them, and links the program. Returns a `Shader`.

#### `shaderUse`
```c
void shaderUse(Shader* shader);
```
Binds this shader program as the active one.

#### Uniform setters
```c
void shaderSetBool  (Shader* shader, const char* name, int value);
void shaderSetInt   (Shader* shader, const char* name, int value);
void shaderSetFloat (Shader* shader, const char* name, float value);
void shaderSetVec2  (Shader* shader, const char* name, vec2 value);
void shaderSetVec3  (Shader* shader, const char* name, vec3 value);
void shaderSetMat4  (Shader* shader, const char* name, mat4 value);
```
Upload a uniform value to the shader by name.

#### `shaderDelete`
```c
void shaderDelete(Shader* shader);
```
Deletes the compiled shader program from the GPU.

---

### `Texture`

#### `createTexture`
```c
Texture createTexture(int unit);
```
Allocates a GPU texture object bound to the given texture unit (e.g. `GL_TEXTURE0`, `GL_TEXTURE1`).

#### `textureLoad`
```c
void textureLoad(Texture* texture, const char* image, int wrap,
                 int minFilter, int maxFilter, int colorType);
```
Loads an image file (relative to the `textures/` directory) and uploads it to the GPU.

| Parameter | Typical values |
|---|---|
| `wrap` | `GL_REPEAT`, `GL_MIRRORED_REPEAT`, `GL_CLAMP_TO_EDGE` |
| `minFilter` | `GL_LINEAR_MIPMAP_LINEAR`, `GL_NEAREST` |
| `maxFilter` | `GL_NEAREST`, `GL_LINEAR` |
| `colorType` | `GL_RGBA`, `GL_RGB` |

#### `textureUse`
```c
void textureUse(Texture* texture);
```
Binds the texture to its unit so it is active for the next draw call.

#### `textureDelete`
```c
void textureDelete(Texture* texture);
```
Deletes the texture from the GPU.

---

### `Window`

You usually do not call these directly — `randererInit` and `randererClose` handle the lifecycle. The `Window` pointer is available when you need the raw `GLFWwindow*` for input polling.

#### `createWindow`
```c
Window createWindow(const char* windowTitle, int windowDimensions[2]);
```
Creates a GLFW window. Called internally by `randererInit`.

#### `windowDelete`
```c
void windowDelete(Window* window);
```
Destroys the GLFW window.

#### `windowSetViewPoint`
```c
void windowSetViewPoint(Window* window);
```
Sets the OpenGL viewport to match the current window dimensions.
