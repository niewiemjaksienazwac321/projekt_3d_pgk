#ifndef CAMERA_H
#define CAMERA_H

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;

    float yaw;
    float pitch;
    float distance; // Odległość kamery od gracza

    Camera(glm::vec3 upVec);

    void apply() const;
    void processMouseMovement(float xoffset, float yoffset);

    // NAPRAWIONE: Tylko czysta deklaracja zakończona średnikiem
    void updateTPP(float playerX, float playerY, float playerZ);
};

#endif