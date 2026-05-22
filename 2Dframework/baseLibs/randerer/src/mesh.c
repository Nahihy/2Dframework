#include <randerer/mesh.h>
#include <glad/glad.h>
Mesh createMesh(float* vertices,unsigned int vertexCount, unsigned int* indices,unsigned int indexCount, VertexAttrib* attrib, unsigned int attribCount) {

  Mesh mesh;
  mesh.vertices = vertices;
  mesh.vertexCount = vertexCount;
  mesh.indices = indices;
  mesh.indexCount = indexCount;

  glGenVertexArrays(1, &mesh.VAO);
  glGenBuffers(1, &mesh.VBO);
  glGenBuffers(1, &mesh.EBO);

  glBindVertexArray(mesh.VAO);

  glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount, mesh.vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, mesh.indices, GL_STATIC_DRAW);

  for(int i = 0; i < attribCount; i++) {
    glVertexAttribPointer(attrib[i].index, attrib[i].size, GL_FLOAT, GL_FALSE, attrib[i].stride, (void*)attrib[i].offset);
    glEnableVertexAttribArray(attrib[i].index);
  }

  glBindVertexArray(0);
  return mesh;
}

void meshUse(Mesh* mesh) {
  glBindVertexArray(mesh->VAO);
}

void meshDraw(Mesh* mesh) {
  glBindVertexArray(mesh->VAO);
  glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void meshDelete(Mesh* mesh) {
  glDeleteVertexArrays(1, &mesh->VAO);
  glDeleteBuffers(1, &mesh->VBO);
  glDeleteBuffers(1, &mesh->EBO);
}

Mesh createSquareMeshTemp() {
  Mesh mesh;
  float vertices[16] = {
   -1.0f, 1.0f,   0.0f, 1.0f,
    1.0f, 1.0f,   1.0f, 1.0f,
   -1.0f,-1.0f,   0.0f, 0.0f,
    1.0f,-1.0f,   1.0f, 0.0f
  };
  unsigned int indices[6] = {
    0, 1, 2,
    1, 2, 3
  };

  VertexAttrib attrib[2] = {
    {0, 2, 4 * sizeof(float), 0},
    {1, 2, 4 * sizeof(float), 2 * sizeof(float)}
  };

  mesh = createMesh(vertices, 16, indices, 6, attrib, 2); 
  return mesh;
}

Mesh createTriangleMeshTemp() {
  Mesh mesh;
  float vertices[12] = {
   -1.0f,-1.0f,   0.0f, 0.0f,
    0.0f, 1.0f,   0.5f, 1.0f,
    1.0f,-1.0f,   1.0f, 0.0f
  };
  unsigned int indices[3] = {
    0, 1, 2,
  };

  VertexAttrib attrib[2] = {
    {0, 2, 4 * sizeof(float), 0},
    {1, 2, 4 * sizeof(float), 2 * sizeof(float)}
  };

  mesh = createMesh(vertices, 12, indices, 3, attrib, 2); 
  return mesh;
}
