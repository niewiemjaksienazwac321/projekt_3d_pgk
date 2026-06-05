#ifndef PLAYER_H
#define PLAYER_H

#include "GeometryProvider.h"

class Player : public CubeObject {
public:
    // Fizyka skoku i grawitacji
    float velocityY;
    bool isGrounded;
    const float GRAVITY = -0.015f;
    const float JUMP_FORCE = 0.32f;

    // ZMIANA: Dodano texTop do ID tekstur kierunkowych
    GLuint texFront, texBack, texLeft, texRight, texTop;

    Player(float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : CubeObject(x, y, z), velocityY(0.0f), isGrounded(true),
          texFront(0), texBack(0), texLeft(0), texRight(0), texTop(0) { // Inicjalizacja zerem
        // Skalowanie postaci gracza
        scaleX = 0.8f;
        scaleY = 2.0f;
        scaleZ = 0.8f;
        // Pozycja startowa nad ziemią
        posY = scaleY / 2.0f;
    }

    // ZMIANA: Metoda przyjmuje teraz również piąty argument dla tekstury góry
    void setTextures(GLuint front, GLuint back, GLuint left, GLuint right, GLuint top) {
        texFront = front;
        texBack = back;
        texLeft = left;
        texRight = right;
        texTop = top;
    }

    void jump() {
        if (isGrounded) {
            velocityY = JUMP_FORCE;
            isGrounded = false;
        }
    }

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

    // Nadpisanie rysowania dla obsługi tekstur kierunkowych
    void draw(bool isShadow = false) override {
        if (!isShadow) {
            glEnable(GL_TEXTURE_2D);
            GLfloat mat_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
            glMaterialf(GL_FRONT, GL_SHININESS, 64.0f);
            glColor3f(1.0f, 1.0f, 1.0f); // Reset koloru bazowego, by tekstura miała naturalne barwy
        } else {
            glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
        }

        // PRZÓD (+Z)
        if (!isShadow) glBindTexture(GL_TEXTURE_2D, texFront);
        glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
        glEnd();

        // TYŁ (-Z)
        if (!isShadow) glBindTexture(GL_TEXTURE_2D, texBack);
        glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, -1.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
        glEnd();

        // PRAWO (+X)
        if (!isShadow) glBindTexture(GL_TEXTURE_2D, texRight);
        glBegin(GL_QUADS);
            glNormal3f(1.0f, 0.0f, 0.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
        glEnd();

        // LEWO (-X)
        if (!isShadow) glBindTexture(GL_TEXTURE_2D, texLeft);
        glBegin(GL_QUADS);
            glNormal3f(-1.0f, 0.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
        glEnd();

        // ZMIANA: GÓRA (+Y) - Teraz poprawnie binduje nową teksturę texTop zamiast texFront
        if (!isShadow) glBindTexture(GL_TEXTURE_2D, texTop);
        glBegin(GL_QUADS);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
        glEnd();

        // DÓŁ (-Y)
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