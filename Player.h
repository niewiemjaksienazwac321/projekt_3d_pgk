/**
 * @file Player.h
 * @brief Definicja klasy sterowanego gracza z obsługą fizyki skoków.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "GeometryProvider.h"

/**
 * @class Player
 * @brief Reprezentuje postać oteksturowanego gracza poruszającego się po scenie.
 */
class Player : public CubeObject {
public:
    float velocityY;          ///< Aktualna prędkość pionowa gracza (fizyka skoku).
    bool isGrounded;          ///< Czy gracz dotyka stopami stabilnej powierzchni.
    const float GRAVITY;      ///< Stałe przyspieszenie grawitacyjne.
    const float JUMP_FORCE;   ///< Siła początkowa impulsu skoku.

    GLuint texFront;          ///< ID tekstury ściany przedniej.
    GLuint texBack;           ///< ID tekstury ściany tylnej.
    GLuint texLeft;           ///< ID tekstury ściany lewej.
    GLuint texRight;          ///< ID tekstury ściany prawej.
    GLuint texTop;            ///< ID tekstury ściany górnej.

    Player(float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : CubeObject(x, y, z), velocityY(0.0f), isGrounded(true),
          GRAVITY(-0.015f), JUMP_FORCE(0.32f),
          texFront(0), texBack(0), texLeft(0), texRight(0), texTop(0) {
        scaleX = 0.8f; scaleY = 2.0f; scaleZ = 0.8f;
        posY = scaleY / 2.0f;
    }

    /**
     * @brief Przypisuje osobne tekstury graficzne kierunkowe do konkretnych ścian kostki gracza.
     */
    void setTextures(GLuint front, GLuint back, GLuint left, GLuint right, GLuint top) {
        texFront = front; texBack = back; texLeft = left; texRight = right; texTop = top;
    }

    /**
     * @brief Nadaje impuls pionowy w celu wywołania skoku postaci.
     */
    void jump() {
        if (isGrounded) {
            velocityY = JUMP_FORCE;
            isGrounded = false;
        }
    }

    /**
     * @brief Aktualizuje przemieszczenie w osi Y w oparciu o siły grawitacji i poziom podłoża.
     * @param customFloorLevel Aktualna wysokość przeszkody pod stopami gracza.
     */
    void updatePhysics(float customFloorLevel) {
        if (!isGrounded) {
            velocityY += GRAVITY;
        }

        posY += velocityY;
        float targetFloorY = customFloorLevel + (scaleY / 2.0f);

        if (posY <= targetFloorY) {
            posY = targetFloorY;
            velocityY = 0.0f;
            isGrounded = true;
        } else {
            if (posY > targetFloorY + 0.001f && velocityY == 0.0f) {
                isGrounded = false;
            }
        }
    }

    void draw(bool isShadow = false) override {
        if (!isShadow) {
            glEnable(GL_TEXTURE_2D);
            GLfloat mat_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
            glMaterialf(GL_FRONT, GL_SHININESS, 64.0f);
            glColor3f(1.0f, 1.0f, 1.0f);
        } else {
            glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
        }

        if (!isShadow) glBindTexture(GL_TEXTURE_2D, texFront);
        glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
        glEnd();

        if (!isShadow) glBindTexture(GL_TEXTURE_2D, texBack);
        glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, -1.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
        glEnd();

        if (!isShadow) glBindTexture(GL_TEXTURE_2D, texRight);
        glBegin(GL_QUADS);
            glNormal3f(1.0f, 0.0f, 0.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
        glEnd();

        if (!isShadow) glBindTexture(GL_TEXTURE_2D, texLeft);
        glBegin(GL_QUADS);
            glNormal3f(-1.0f, 0.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
        glEnd();

        if (!isShadow) glBindTexture(GL_TEXTURE_2D, texTop);
        glBegin(GL_QUADS);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
        glEnd();

        if (!isShadow) glBindTexture(GL_TEXTURE_2D, texFront);
        glBegin(GL_QUADS);
            glNormal3f(0.0f, -1.0f, 0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
        glEnd();

        if (!isShadow) glDisable(GL_TEXTURE_2D);
    }
};

#endif