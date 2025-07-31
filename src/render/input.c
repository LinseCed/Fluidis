#include "input.h"

bool first_mouse = true;
float lastX = INITIAL_SCREEN_WIDTH / 2.0f;
float lastY = INITIAL_SCREEN_HEIGHT / 2.0f;

void process_input(GLFWwindow* window, Camera* camera, float delta) {
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

    if (first_mouse) {
        lastX = xposIn;
        lastY = yposIn;
        first_mouse = false;
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
    if (height == 0) {
        height = 1;
    }
    aspect_ratio = (float) width / (float) height;
}
