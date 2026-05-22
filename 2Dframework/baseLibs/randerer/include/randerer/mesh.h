typedef struct {
  unsigned int index;
  unsigned int size;
  unsigned int stride;
  long offset;
} VertexAttrib;


typedef struct {
  float* vertices;
  unsigned int vertexCount;
  unsigned int* indices;
  unsigned int indexCount;
  unsigned int VAO, VBO, EBO;
} Mesh;
Mesh createMesh(float* vertices,unsigned int vertexCount, unsigned int* indices, unsigned int indexCount,
                VertexAttrib* attrib, unsigned int attribCount);
void meshChangeVerticies(Mesh* mesh, float* verticies);
void meshUse(Mesh* mesh);
void meshDraw(Mesh* mesh);
void meshDelete(Mesh* mesh);
Mesh createSquareMeshTemp();
Mesh createTriangleMeshTemp();
