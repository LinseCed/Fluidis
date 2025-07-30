#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "render/model.h"
#include "render/glbparser.h"
#include "render/shader.h"

int main() {
    if (!glfwInit()) {
        printf("Error initalizing GLFW");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "FLUIDIS", NULL, NULL);
    if(!window) {
        printf("Error initalizing Window");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);

    Model* model = parse_glb("../test/models/cube/Cube.glb");

    Shader shader = shader_create("../rsc/shader/shader.vert", "../rsc/shader/shader.frag");

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader_use(&shader);
        mesh_draw(model->meshes[0]);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shader_destroy(&shader);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}


