#include "model.h"
#include <stdlib.h>
#include <stdio.h>

Model* create_model() {
    Model* model = (Model*) malloc(sizeof(Model));
    model->meshcount = 0;
    model->meshes = NULL;
    return model;
}

void destory_model(Model* model) {
    if (!model) return;
    for (size_t i = 0; i < model->meshcount; i++) {
        mesh_destroy(model->meshes[i]);
    }
    free(model->meshes);
    free(model);
}

void add_mesh(Model* model, Mesh* mesh) {
    if (!model || !mesh) return;
    size_t new_mesh_count = model->meshcount + 1;
    Mesh** meshes = realloc(model->meshes, new_mesh_count);
    if (!meshes) {
        printf("Failed to add mesh to model");
        return;
    }
    model->meshes = meshes;
    model->meshcount = new_mesh_count;
    model->meshes[model->meshcount - 1] = mesh;
    return;
}
