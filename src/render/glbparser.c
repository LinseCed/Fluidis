#include "glbparser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mesh.h"

Model* parse_glb(char* path) {
    Model* model = create_model();
    FILE* file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open glb file: %s\n", path);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    unsigned char* buffer = malloc(size);
    if (!buffer) return NULL;

    fread(buffer, 1, size, file);
    fclose(file);

    if (size < sizeof(GlbHeader)) {
        fprintf(stderr, "File is to small to be in GLB Format\n");
        free(buffer);
        free(model);
        return NULL;
    }

    GlbHeader* header = (GlbHeader*) buffer;
    if (header->magic != 0x46546C67) {
        fprintf(stderr, "Invalid GLB header/Header not found \n");
        free(buffer);
        free(model);
        return NULL;
    }

    size_t offset = sizeof(GlbHeader);
    char* jsonString = NULL;
    char* binString = NULL;

    while (offset + sizeof(GlbChunkHeader) <= (size_t) size) {
        GlbChunkHeader* chunk = (GlbChunkHeader*) (buffer + offset);
        offset += sizeof(GlbChunkHeader);

        if (offset + chunk->chunklength > (size_t) size) {
            fprintf(stderr, "Chunk length out of bounds (offset: %zu, chunklength %u, size %lu)\n", offset, chunk->chunklength, size);
            free(buffer);
            free(model);
            return NULL;
        }

        unsigned char* chunk_data = buffer + offset;

        if (chunk->chunkType == 0x4E4F534A) { //JSON in binary
            jsonString = (char*) malloc(chunk->chunklength + 1);
            memcpy(jsonString, chunk_data, chunk->chunklength);
            jsonString[chunk->chunklength] = '\0';
        } else if (chunk->chunkType == 0x004E4942) { //BIN in binary
            binString = (char*) malloc(chunk->chunklength);
            memcpy(binString, chunk_data, chunk->chunklength);
        }

        offset += chunk->chunklength;
    }

    if (!jsonString || !binString) {
        fprintf(stderr, "Failed to read");
        free(buffer);
        free(model);
        free(jsonString);
        free(binString);
        return NULL;
    }

    cJSON* root = cJSON_Parse(jsonString);
    if (!root) {
        fprintf(stderr, "Failed to parse JSON");
        free(buffer);
        free(model);
        free(jsonString);
        free(binString);
        return NULL;
    }

    cJSON* accessors = cJSON_GetObjectItem(root, "accessors");
    cJSON* bufferViews = cJSON_GetObjectItem(root, "bufferViews");


    cJSON* posAccessor = cJSON_GetArrayItem(accessors, 0);
    int posBufferViewIndex = cJSON_GetObjectItem(posAccessor, "bufferView")->valueint;
    int posCount = cJSON_GetObjectItem(posAccessor, "count")->valueint;
    cJSON* posBufferView = cJSON_GetArrayItem(bufferViews, posBufferViewIndex);
    int posByteOffset = cJSON_GetObjectItem(posBufferView, "byteOffset")->valueint;

    cJSON* indexAccessor = cJSON_GetArrayItem(accessors, 3);
    int indexBufferViewIndex = cJSON_GetObjectItem(indexAccessor, "bufferView")->valueint;
    int indexCount = cJSON_GetObjectItem(indexAccessor, "count")->valueint;
    cJSON* indexBufferView = cJSON_GetArrayItem(bufferViews, indexBufferViewIndex);
    int indexByteOffset = cJSON_GetObjectItem(indexBufferView, "byteOffset")->valueint;

    const float* posDataLocation = (const float*) (binString + posByteOffset);

    Vertex vertices[posCount];
    for (size_t i = 0; i < posCount; i++) {
        for (size_t j = 0; j < 3; j++) {
            vertices[i].position[j] = posDataLocation[i * 3 + j];
        }
    }

    const unsigned short* indexDataLocation = (const unsigned short*) (binString + indexByteOffset);

    unsigned int indices[indexCount];
    for (size_t i = 0; i < indexCount; i++) {
        indices[i] = indexDataLocation[i];
    }

    Mesh* mesh = mesh_create(vertices, posCount, indices, indexCount);

    add_mesh(model, mesh);

    free(buffer);
    free(jsonString);
    free(binString);
    return model;
}

