#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

#define CAMERA_YAW          -90.0f
#define CAMERA_PITCH        0.0f
#define CAMERA_SPEED        2.5f
#define CAMERA_SENSITIVITY  0.1f
#define CAMERA_ZOOM         45.0f


typedef struct {
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 worldUp;

    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
    float zoom;
} Camera;

Camera create_camera(vec3 position, vec3 up);
void camera_update_vectors(Camera* camera);
void camera_get_view_matrix(Camera* camera, mat4 dest);
void camera_process_input(Camera* camera, enum CameraMovement dir, float delta);
void camera_get_projection_matrix(Camera* camera, float aspect, float near, float far, mat4 dest);
#endif
