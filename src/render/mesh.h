#ifndef MESH_H
#define MESH_H

#include "vertex.h"

typedef struct {
    Vertex* vertices;
    unsigned int* indices;
    unsigned int vertexCount;
    unsigned int indexCount;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
} Mesh;

Mesh* mesh_create(Vertex* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount);
void mesh_destroy(Mesh* mesh);
void mesh_draw(Mesh* mesh);
void print(Mesh* mesh);
#endif
