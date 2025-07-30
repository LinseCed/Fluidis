#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "render/model.h"
#include "render/glbparser.h"
#include "render/shader.h"
#include "render/camera.h"

#define SCREEN_HEIGHT   600
#define SCREEN_WIDTH    800
#define ASPECT_RATIO    SCREEN_WIDTH / SCREEN_HEIGHT

float delta = 0.0f;
float lastFrame = 0.0f;
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

void processInput(GLFWwindow* window, Camera* camera, float delta) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera_process_input(camera, FORWARD, delta);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera_process_input(camera, LEFT, delta);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera_process_input(camera, BACKWARD, delta);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera_process_input(camera, RIGHT, delta);
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = (float) xposIn;
    float ypos = (float) yposIn;

    if (firstMouse) {
        lastX = xposIn;
        lastY = yposIn;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    Camera* camera = (Camera*) glfwGetWindowUserPointer(window);
    if (camera != NULL) {
        camera_process_mouse(camera, xoffset, yoffset);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    if (!glfwInit()) {
        printf("Error initalizing GLFW");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "FLUIDIS", NULL, NULL);
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

        processInput(window, camera, delta);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        shader_use(&shader);

        camera_get_projection_matrix(camera, ASPECT_RATIO, 0.1f, 100.0f, projection);
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


