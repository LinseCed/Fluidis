#ifndef SHADER_H
#define SHADER_H

typedef struct {
    unsigned int id;
} Shader;

Shader shader_create(const char* vertexPath, const char* fragmentPath);
void shader_use(Shader* shader);
void shader_destroy(Shader* shader);

void shader_set_uniform_mat4(Shader* shader, const char* name, const float* mat);
void shader_set_uniform_int(Shader* shader, const char* name, int value);
void shader_set_uniform_float(Shader* shader, const char* name, float value);
#endif
