#include "camera.h"
#include <stdlib.h>
#include <stdbool.h>

void camera_update_vectors(Camera* camera) {
    vec3 front;
    front[0] = cosf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));
    front[1] = sinf(glm_rad(camera->pitch));
    front[2] = sinf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));
    glm_normalize(front);
    glm_vec3_copy(front, camera->front);

    glm_cross(camera->front, camera->worldUp, camera->right);
    glm_normalize(camera->right);

    glm_cross(camera->right, camera->front, camera->up);
    glm_normalize(camera->up);
}

Camera* camera_create(vec3 position, vec3 up) {
    Camera* camera = malloc(sizeof(Camera));
    glm_vec3_copy(position, camera->position);
    glm_vec3_copy(up, camera->worldUp);
    camera->yaw = CAMERA_YAW;
    camera->pitch = CAMERA_PITCH;
    camera->movementSpeed = CAMERA_SPEED;
    camera->mouseSensitivity = CAMERA_SENSITIVITY;
    camera->zoom = CAMERA_ZOOM;
    vec3 front = {0.0f, 0.0f, -1.0f};
    glm_vec3_copy(front, camera->front);
    camera_update_vectors(camera);
    return camera;
}
void camera_get_view_matrix(Camera* camera, mat4 dest) {
    vec3 center;
    glm_vec3_add(camera->position, camera->front, center);
    glm_lookat(camera->position, center, camera->up, dest);
}
void camera_process_input(Camera* camera, enum CameraMovement dir, float delta) {
    float velocity = camera->movementSpeed * delta;
    vec3 temp;
    if (dir == FORWARD) {
        glm_vec3_scale(camera->front, velocity, temp);
        glm_vec3_add(camera->position, temp, camera->position);
    }

    if (dir == BACKWARD) {
        glm_vec3_scale(camera->front, velocity, temp);
        glm_vec3_sub(camera->position, temp, camera->position);
    }

    if (dir == LEFT) {
        glm_vec3_scale(camera->right, velocity, temp);
        glm_vec3_sub(camera->position, temp, camera->position);
    }

    if (dir == RIGHT) {
        glm_vec3_scale(camera->right, velocity, temp);
        glm_vec3_add(camera->position, temp, camera->position);
    }
}

void camera_get_projection_matrix(Camera* camera, float aspect, float near, float far, mat4 dest) {
    float fov = glm_rad(camera->zoom);
    glm_perspective(fov, aspect, near, far, dest);
}

void camera_process_mouse_movement(Camera* camera, float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= camera->mouseSensitivity;
    yoffset *= camera->mouseSensitivity;
    
    camera->yaw += xoffset;
    camera->pitch += yoffset;
    
    if (constrainPitch) {
        if (camera->pitch > 89.0f)
            camera->pitch = 89.0f;
        if (camera->pitch < -89.0f)
            camera->pitch = -89.0f;
    }
    
    camera_update_vectors(camera);
}
