#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"

Camera::Camera() {
    position = glm::vec3(0, 0, 0);
    target = glm::vec3(0, 0, 0);
    up = glm::vec3(0, 1, 0);
}
glm::mat4 Camera::GetViewProjMatrix() const {
    glm::mat4 view = glm::lookAt(position, target, up);
    return proj * view;
}

void Camera::perspective(int width, int height) {
    proj = glm::perspective(glm::radians(45.0f), width / (float)height, 1.0f, 1000.0f);
}