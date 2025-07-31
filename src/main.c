#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "render/model.h"
#include "render/glbparser.h"
#include "render/shader.h"
#include "render/camera.h"
#include "render/input.h"
#include "render/window.h"

float delta = 0.0f;
float lastFrame = 0.0f;

int main() {
    if (!glfwInit()) {
        printf("Error initalizing GLFW");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    GLFWwindow* window = glfwCreateWindow(INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT, "FLUIDIS", NULL, NULL);
    if(!window) {
        printf("Error initalizing Window");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    vec3 position = {0.0f, 0.0f, 3.0f};
    vec3 up = {0.0f, 1.0f, 0.0f};
    Camera* camera = create_camera(position, up);
    glfwSetWindowUserPointer(window, camera);
    glfwSetCursorPosCallback(window, mouse_callback);
    Model* cube = parse_glb("../test/models/cube/Cube.glb");

    Shader shader = shader_create("../rsc/shader/shader.vert", "../rsc/shader/shader.frag");

    mat4 projection;
    mat4 view;
    mat4 model;
    glm_mat4_identity(model);
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float) glfwGetTime();
        delta = currentFrame - lastFrame;
        lastFrame = currentFrame;

        process_input(window, camera, delta);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        shader_use(&shader);

        camera_get_projection_matrix(camera, aspect_ratio, 0.1f, 100.0f, projection);
        shader_set_uniform_mat4(&shader, "projection", (const float*) projection);

        camera_get_view_matrix(camera, view);
        shader_set_uniform_mat4(&shader, "view", (const float*) view);

        shader_set_uniform_mat4(&shader, "model", (const float*) model);

        mesh_draw(cube->meshes[0]);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shader_destroy(&shader);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}


