#include "Camera.h"
#include <cmath>

Camera::Camera(glm::vec3 upVec)
    : up(upVec), yaw(-90.0f), pitch(15.0f), distance(6.0f), position(0.0f), target(0.0f) {}

void Camera::apply() const {
    glm::mat4 view = glm::lookAt(position, target, up);
    glLoadMatrixf(glm::value_ptr(view));
}

void Camera::processMouseMovement(float xoffset, float yoffset) {
    float sensitivity = 0.15f;
    yaw += xoffset * sensitivity;
    pitch += yoffset * sensitivity;

    // Ograniczenia rotacji góra/dół
    if (pitch > 85.0f) pitch = 85.0f;  // Widok z lotu ptaka (z góry)
    if (pitch < -15.0f) pitch = -15.0f; // NAPRAWIONE: Pozwala opuścić kamerę na poziom ziemi i patrzeć lekko w górę
}

void Camera::updateTPP(float playerX, float playerY, float playerZ) {
    // Punkt celowania (środek postaci gracza)
    target = glm::vec3(playerX, playerY + 1.0f, playerZ);

    // Pozycja kamery wokół punktu docelowego
    position.x = playerX + distance * std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    position.y = playerY + 1.0f + distance * std::sin(glm::radians(pitch));
    position.z = playerZ + distance * std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
}