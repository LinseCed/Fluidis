#include "mesh.h"
#include <stdlib.h>
#include <string.h>
#include <glad/glad.h>
#include <stddef.h>

Mesh* mesh_create(Vertex* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount) {
    Mesh* mesh = (Mesh*) malloc(sizeof(Mesh));
    if (!mesh) return NULL;

    mesh->vertexCount = vertexCount;
    mesh->indexCount = indexCount;

    mesh->vertices = (Vertex*) malloc(sizeof(Vertex) * vertexCount);
    memcpy(mesh->vertices, vertices, sizeof(Vertex) * vertexCount);

    mesh->indices = (unsigned int*) malloc(sizeof(unsigned int) * indexCount);
    memcpy(mesh->indices, indices, sizeof(unsigned int) * indexCount);

    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    glBindVertexArray(mesh->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexCount, mesh->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, mesh->indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (offsetof(Vertex, texCoords)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    return mesh;
}

void mesh_destroy(Mesh* mesh) {
    if (!mesh) return;
    glDeleteVertexArrays(1, &mesh->VAO);
    glDeleteBuffers(1, &mesh->VBO);
    glDeleteBuffers(1, &mesh->EBO);

    free(mesh->vertices);
    free(mesh->indices);
    free(mesh);
}

void mesh_draw(Mesh* mesh) {
    if (!mesh) return;
    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

