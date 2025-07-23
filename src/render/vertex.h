#ifndef VERTEX_H
#define VERTEX_H

typedef struct {
    float position[3];
    float normal[3];
    float texCoords[2];

    float tangent[3];
    float bitangent[3];
} Vertex;

#endif
