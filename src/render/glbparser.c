#include "glbparser.h"
#include <stdio.h>
#include <stdlib.h>

Model* parse_glb(char* path) {
    Model* model = (Model*) malloc(sizeof(Model));
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

    for (size_t i = 0; i < size; i++) {
        unsigned char c = buffer[i];
        printf("%02X", c);
        if ((i + 1) % 16 == 0) printf("\n");
    }

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

    printf("GLB Version: %u\n", header->version);
    printf("Total length: %u bytes\n", header->length);

    size_t offset = sizeof(GlbHeader);

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
            printf("JSON chunk found\n");
            cJSON* root = cJSON_Parse((const char*) chunk_data);
            if (!root) {
                fprintf(stderr, "Failed to parse JSON");
                free(buffer);
                free(model);

                return NULL;
            }
            cJSON* meshes = cJSON_GetObjectItem(root, "meshes");
            if (meshes && cJSON_IsArray(meshes)) {
                int mesh_count = cJSON_GetArraySize(meshes);
                for (int i = 0; i < mesh_count; i++) {
                    cJSON* mesh = cJSON_GetArrayItem(meshes, i);
                    cJSON* name = cJSON_GetObjectItem(mesh, "name");
                    if (name && cJSON_IsString(name)) {
                        printf("Mesh[%d]: %s\n", i, name->valuestring);
                    }
                }
            }
        } else if (chunk->chunkType == 0x004E4942) { //BIN in binary
            printf("BIN chunk found\n");
        }

        offset += chunk->chunklength;
    }
}
