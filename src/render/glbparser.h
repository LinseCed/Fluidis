#ifndef GLB_PARSER_H
#define GLB_PARSER_H

#include "model.h"
#include <stdint.h>
#include <cjson/cJSON.h>

#pragma pack(push, 1)
typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t length;
} GlbHeader;

typedef struct {
    uint32_t chunklength;
    uint32_t chunkType;
} GlbChunkHeader;
#pragma pack(pop)

Model* parse_glb(char* path);

#endif
