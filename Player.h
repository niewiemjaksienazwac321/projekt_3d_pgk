/**
 * @file Player.h
 * @brief Klasa gracza z przywróconą, stabilną animacją kończyn i głową HeadObject.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "GeometryProvider.h"
#include <cmath>
#include <algorithm>
#include <glm/glm.hpp>

class Player : public Object3D {
public:
    float velocityY;
    bool isGrounded;
    const float GRAVITY;
    const float JUMP_FORCE;

    float walkCycle;
    glm::vec3 hVelocity;

    GLuint texLeg, texArm, texTorsoFront, texTorsoBack, texTorsoLeft, texTorsoRight, texTorsoTop;
    GLuint texHeadFront, texHeadBack;
    HeadObject head;

    Player(float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : Object3D(x, y, z), velocityY(0.0f), isGrounded(true),
          GRAVITY(-0.015f), JUMP_FORCE(0.32f), walkCycle(0.0f),
          hVelocity(0.0f, 0.0f, 0.0f),
          texLeg(0), texArm(0), texTorsoFront(0), texTorsoBack(0),
          texTorsoLeft(0), texTorsoRight(0), texTorsoTop(0),
          texHeadFront(0), texHeadBack(0), head(0.0f, 0.0f, 0.0f) {
        scaleX = 0.8f; scaleY = 2.0f; scaleZ = 0.8f;
        posY = scaleY / 2.0f;
    }

    void setTextures(GLuint leg, GLuint arm, GLuint tFront, GLuint tBack, GLuint tLeft, GLuint tRight, GLuint tTop, GLuint hFront, GLuint hBack) {
        texLeg = leg; texArm = arm;
        texTorsoFront = tFront; texTorsoBack = tBack;
        texTorsoLeft = tLeft; texTorsoRight = tRight; texTorsoTop = tTop;
        texHeadFront = hFront; texHeadBack = hBack;

        head.textureId = hFront;
    }

    void jump() {
        if (isGrounded) {
            velocityY = JUMP_FORCE;
            isGrounded = false;
        }
    }

    void updateMovementPhysics(bool isMoving, glm::vec3 inputDir, bool isOnIce, bool isOnIceSlope) {
        float maxSpeed = 0.12f;
        glm::vec3 targetVel = isMoving ? (inputDir * maxSpeed) : glm::vec3(0.0f);

        float accelFactor;
        if (isOnIce) {
            bool changingDirection = isMoving && (glm::length(hVelocity) > 0.001f) && (glm::dot(glm::normalize(hVelocity), inputDir) < -0.1f);
            accelFactor = changingDirection ? 0.15f : (isMoving ? 0.04f : 0.012f);
        } else {
            accelFactor = isMoving ? 0.22f : 0.18f;
        }

        hVelocity = glm::mix(hVelocity, targetVel, accelFactor);

        if (isOnIceSlope) {
            hVelocity.z -= 0.0025f;
        }
    }

    void updateAnimation() {
        float speed = glm::length(hVelocity);
        if (speed > 0.002f) {
            walkCycle += speed * 2.0f; // Przywrócona oryginalna, normalna prędkość machania
        } else {
            walkCycle = 0.0f;
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
            if (posY - targetFloorY < 0.25f) {
                posY = targetFloorY;
                velocityY = 0.0f;
                isGrounded = true;
            } else if (posY > targetFloorY + 0.001f && velocityY == 0.0f) {
                isGrounded = false;
            }
        }
    }

    void drawBox(float width, float height, float depth, GLuint textureID, bool isShadow) {
        if (!isShadow) {
            glEnable(GL_TEXTURE_2D);
            if (textureID != 0) {
                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }
            GLfloat mat_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
            glMaterialf(GL_FRONT, GL_SHININESS, 64.0f);
            glColor3f(1.0f, 1.0f, 1.0f);
        } else {
            glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
        }

        float hw = width / 2.0f;
        float hd = depth / 2.0f;

        glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, 1.0f);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f(-hw, 0.0f,  hd);
            if (!isShadow) glTexCoord2f(1.0f, 0.0f); glVertex3f( hw, 0.0f,  hd);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f( hw, -height,  hd);
            if (!isShadow) glTexCoord2f(0.0f, 1.0f); glVertex3f(-hw, -height,  hd);
        glEnd();

        glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, -1.0f);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f(-hw, 0.0f, -hd);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f(-hw, -height, -hd);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f( hw, -height, -hd);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f( hw, 0.0f, -hd);
        glEnd();

        glBegin(GL_QUADS);
            glNormal3f(1.0f, 0.0f, 0.0f);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f( hw, 0.0f, -hd);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f( hw, -height, -hd);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f( hw, -height,  hd);
            if (!isShadow) glTexCoord2f(0.0f, 1.0f); glVertex3f( hw, 0.0f,  hd);
        glEnd();

        glBegin(GL_QUADS);
            glNormal3f(-1.0f, 0.0f, 0.0f);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f(-hw, 0.0f, -hd);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f(-hw, 0.0f,  hd);
            if (!isShadow) glTexCoord2f(0.0f, 1.0f); glVertex3f(-hw, -height,  hd);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f(-hw, -height, -hd);
        glEnd();

        glBegin(GL_QUADS);
            glNormal3f(0.0f, 1.0f, 0.0f);
            if (!isShadow) glTexCoord2f(0.0f, 1.0f); glVertex3f(-hw, 0.0f, -hd);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f(-hw, 0.0f,  hd);
            if (!isShadow) glTexCoord2f(1.0f, 0.0f); glVertex3f( hw, 0.0f,  hd);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f( hw, 0.0f, -hd);
        glEnd();

        glBegin(GL_QUADS);
            glNormal3f(0.0f, -1.0f, 0.0f);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f(-hw, -height, -hd);
            if (!isShadow) glTexCoord2f(0.0f, 1.0f); glVertex3f( hw, -height, -hd);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f( hw, -height,  hd);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f(-hw, -height,  hd);
        glEnd();

        if (!isShadow) glDisable(GL_TEXTURE_2D);
    }

    void draw(bool isShadow = false) override {
        glPushMatrix();
        glScalef(1.0f / scaleX, 1.0f / scaleY, 1.0f / scaleZ);

        float maxAngle = 35.0f;
        float legAngle = std::sin(walkCycle) * maxAngle;
        float armAngle = -legAngle;

        float legWidth = 0.22f, legHeight = 1.0f, legDepth = 0.22f;
        float armWidth = 0.2f, armHeight = 0.9f, armDepth = 0.22f;
        float torsoWidth = 0.5f, torsoHeight = 1.0f, torsoDepth = 0.35f;

        // 1. LEWA NOGA
        glPushMatrix();
        glTranslatef(-0.12f, 0.0f, 0.0f);
        glRotatef(legAngle, 1.0f, 0.0f, 0.0f);
        drawBox(legWidth, legHeight, legDepth, texLeg, isShadow);
        glPopMatrix();

        // 2. PRAWA NOGA
        glPushMatrix();
        glTranslatef(0.12f, 0.0f, 0.0f);
        glRotatef(-legAngle, 1.0f, 0.0f, 0.0f);
        drawBox(legWidth, legHeight, legDepth, texLeg, isShadow);
        glPopMatrix();

        // 3. TUŁÓW
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.0f);

        if (!isShadow) {
            glEnable(GL_TEXTURE_2D);
            if (texTorsoFront != 0) {
                glBindTexture(GL_TEXTURE_2D, texTorsoFront);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }
            GLfloat mat_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
            glMaterialf(GL_FRONT, GL_SHININESS, 64.0f);
            glColor3f(1.0f, 1.0f, 1.0f);
        } else {
            glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
        }

        float thw = torsoWidth / 2.0f;
        float thd = torsoDepth / 2.0f;

        if (!isShadow) glBindTexture(GL_TEXTURE_2D, texTorsoFront);
        glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, 1.0f);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f(-thw, 0.0f,  thd);
            if (!isShadow) glTexCoord2f(1.0f, 0.0f); glVertex3f( thw, 0.0f,  thd);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f( thw, torsoHeight,  thd);
            if (!isShadow) glTexCoord2f(0.0f, 1.0f); glVertex3f(-thw, torsoHeight,  thd);
        glEnd();

        if (!isShadow) glBindTexture(GL_TEXTURE_2D, texTorsoBack);
        glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, -1.0f);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f(-thw, 0.0f, -thd);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f(-thw, torsoHeight, -thd);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f( thw, torsoHeight, -thd);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f( thw, 0.0f, -thd);
        glEnd();

        if (!isShadow) glBindTexture(GL_TEXTURE_2D, texTorsoRight);
        glBegin(GL_QUADS);
            glNormal3f(1.0f, 0.0f, 0.0f);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f( thw, 0.0f, -thd);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f( thw, torsoHeight, -thd);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f( thw, torsoHeight,  thd);
            if (!isShadow) glTexCoord2f(0.0f, 1.0f); glVertex3f( thw, 0.0f,  thd);
        glEnd();

        if (!isShadow) glBindTexture(GL_TEXTURE_2D, texTorsoLeft);
        glBegin(GL_QUADS);
            glNormal3f(-1.0f, 0.0f, 0.0f);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f(-thw, 0.0f, -thd);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f(-thw, 0.0f,  thd);
            if (!isShadow) glTexCoord2f(0.0f, 1.0f); glVertex3f(-thw, torsoHeight,  thd);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f(-thw, torsoHeight, -thd);
        glEnd();

        if (!isShadow) glBindTexture(GL_TEXTURE_2D, texTorsoTop);
        glBegin(GL_QUADS);
            glNormal3f(0.0f, 1.0f, 0.0f);
            if (!isShadow) glTexCoord2f(0.0f, 1.0f); glVertex3f(-thw, torsoHeight, -thd);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f(-thw, torsoHeight,  thd);
            if (!isShadow) glTexCoord2f(1.0f, 0.0f); glVertex3f( thw, torsoHeight,  thd);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f( thw, torsoHeight, -thd);
        glEnd();

        if (!isShadow) glBindTexture(GL_TEXTURE_2D, texTorsoFront);
        glBegin(GL_QUADS);
            glNormal3f(0.0f, -1.0f, 0.0f);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f(-thw, 0.0f, -thd);
            if (!isShadow) glTexCoord2f(0.0f, 1.0f); glVertex3f( thw, 0.0f, -thd);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f( thw, 0.0f,  thd);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f(-thw, 0.0f,  thd);
        glEnd();

        if (!isShadow) glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        // 4. LEWA RĘKA
        glPushMatrix();
        glTranslatef(-0.32f, 1.0f, 0.0f);
        glRotatef(armAngle, 1.0f, 0.0f, 0.0f);
        drawBox(armWidth, armHeight, armDepth, texArm, isShadow);
        glPopMatrix();

        // 5. PRAWA RĘKA
        glPushMatrix();
        glTranslatef(0.32f, 1.0f, 0.0f);
        glRotatef(-armAngle, 1.0f, 0.0f, 0.0f);
        drawBox(armWidth, armHeight, armDepth, texArm, isShadow);
        glPopMatrix();

        // 6. GŁOWA
        glPushMatrix();
        glTranslatef(0.0f, 1.225f, 0.0f);
        glScalef(0.45f, 0.45f, 0.45f);
        head.draw(isShadow);
        glPopMatrix();

        glPopMatrix();
    }
};

#endif