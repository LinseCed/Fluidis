#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include "camera.h"
#include "window.h"

extern bool first_mouse;
extern float lastX;
extern float lastY;

void process_input(GLFWwindow* window, Camera* camera, float delta);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif
