/**
* @file Camera.h
 * @brief Definicja klasy kamery trzecioosobowej (TPP).
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * @class Camera
 * @brief Reprezentuje sferyczny układ kamery śledzącej ruchy gracza.
 */
class Camera {
public:
    glm::vec3 position; ///< Pozycja oka kamery XYZ w przestrzeni.
    glm::vec3 target;   ///< Punkt docelowy, na który skierowany jest obiektyw.
    glm::vec3 up;       ///< Wektor kierunku góry (zwykle [0, 1, 0]).

    float yaw;          ///< Obrót kamery wokół osi pionowej (lewo/prawo).
    float pitch;        ///< Pochylenie kamery góra/dół.
    float distance;     ///< Promień odległości od celu śledzenia.

    /**
     * @brief Konstruktor klasy kamery.
     * @param upVec Początkowy pionowy wektor odniesienia.
     */
    Camera(glm::vec3 upVec);

    /**
     * @brief Przekazuje wyliczoną macierz widoku do potoku stałego OpenGL.
     */
    void apply() const;

    /**
     * @brief Przelicza kąty sferyczne na bazie przemieszczenia myszy.
     * @param xoffset Zmiana położenia myszy w poziomie.
     * @param yoffset Zmiana położenia myszy w pionie.
     */
    void processMouseMovement(float xoffset, float yoffset);

    /**
     * @brief Aktualizuje położenie sferyczne kamery względem aktualnych współrzędnych gracza.
     * @param playerX Pozycja X gracza.
     * @param playerY Pozycja Y gracza.
     * @param playerZ Pozycja Z gracza.
     */
    void updateTPP(float playerX, float playerY, float playerZ);
};

#endif