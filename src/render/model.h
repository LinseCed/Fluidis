#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"

typedef struct {
    Mesh** meshes;
    unsigned int meshcount;
} Model;

Model* create_model();
void destroy_model(Model* model);
void add_mesh(Model* model, Mesh* mesh);

#endif
