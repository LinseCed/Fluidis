#include "shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <string.h>

static char* read_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* buffer = malloc(size + 1);
    if (!buffer) return NULL;

    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    fclose(file);
    return buffer;
}

static void check_compile_errors(unsigned int shader, const char* type) {
    int success;
    char infoLog[512];
    if (strcmp(type, "PROGRAM") == 0) {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 512, NULL, infoLog);
            fprintf(stderr, "ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n", type, infoLog);
        }
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            fprintf(stderr, "ERROR::SHADER_COMPILING_ERROR of type: %s\n%s\n", type, infoLog);
        }
    }
}

Shader create_shader(const char* vertexPath, const char* fragmentPath) {
    Shader shader = {0};

    char* vertexCode = read_file(vertexPath);
    char* fragmentCode = read_file(fragmentPath);

    if (!vertexCode || !fragmentCode) {
        free(vertexCode);
        free(fragmentCode);
        return shader;
    }

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (const char**)&vertexCode, NULL);
    glCompileShader(vertex);

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const char**)&fragmentCode, NULL);
    glCompileShader(fragment);

    shader.id = glCreateProgram();
    glAttachShader(shader.id, vertex);
    glAttachShader(shader.id, fragment);
    glLinkProgram(shader.id);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    free(vertexCode);
    free(fragmentCode);

    return shader;
}

void shader_use(Shader* shader) {
    glUseProgram(shader->id);
}

void shader_destroy(Shader* shader) {
    glDeleteProgram(shader->id);
    shader->id = 0;
}

void shader_set_uniform_mat4(Shader* shader, const char* name, const float* mat) {
    int loc = glGetUniformLocation(shader->id, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, mat);
}

void shader_set_uniform_int(Shader* shader, const char* name, int value) {
    int loc = glGetUniformLocation(shader->id, name);
    glUniform1i(loc, value);
}

void shader_set_uniform_float(Shader* shader, const char* name, float value) {
    int loc = glGetUniformLocation(shader->id, name);
    glUniform1f(loc, value);
}
