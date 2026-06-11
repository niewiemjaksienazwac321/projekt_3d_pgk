/**
 * @file GeometryProvider.cpp
 * @brief Definicje struktur tablic wierzchołków i implementacja renderowania brył.
 */

#include "GeometryProvider.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Object3D::Object3D(float x, float y, float z)
    : posX(x), posY(y), posZ(z), rotX(0), rotY(0), rotZ(0),
      scaleX(1.0f), scaleY(1.0f), scaleZ(1.0f), textureId(0) {}

void Object3D::applyTransformations() const {
    glTranslatef(posX, posY, posZ);
    glRotatef(rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotZ, 0.0f, 0.0f, 1.0f);
    glScalef(scaleX, scaleY, scaleZ);
}

void TriangleObject::draw(bool isShadow) {
    const float vertices[] = { 0.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f };
    const float colors[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    const float normals[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    if (!isShadow) {
        GLfloat mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, 32.0f);

        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glColorPointer(3, GL_FLOAT, 0, colors);
        glNormalPointer(GL_FLOAT, 0, normals);
    } else {
        glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
    }

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableClientState(GL_VERTEX_ARRAY);
    if (!isShadow) { glDisableClientState(GL_COLOR_ARRAY); glDisableClientState(GL_NORMAL_ARRAY); }
}

void CubeObject::draw(bool isShadow) {
    if (textureId == 0) {
        const float vertices[] = {
            -0.5f, -0.5f,  0.5f,   0.5f, -0.5f,  0.5f,   0.5f,  0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,  -0.5f,  0.5f, -0.5f,   0.5f,  0.5f, -0.5f,   0.5f, -0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,  -0.5f,  0.5f,  0.5f,   0.5f,  0.5f,  0.5f,   0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,   0.5f, -0.5f, -0.5f,   0.5f, -0.5f,  0.5f,  -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f, -0.5f,   0.5f,  0.5f, -0.5f,   0.5f,  0.5f,  0.5f,   0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,  -0.5f, -0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,  -0.5f,  0.5f, -0.5f
        };

        const float colors[] = {
            1,0,0, 1,0,0, 1,0,0, 1,0,0,
            0,1,0, 0,1,0, 0,1,0, 0,1,0,
            0,0,1, 0,0,1, 0,0,1, 0,0,1,
            1,1,0, 1,1,0, 1,1,0, 1,1,0,
            1,0,1, 1,0,1, 1,0,1, 1,0,1,
            0,1,1, 0,1,1, 0,1,1, 0,1,1
        };

        const float normals[] = {
             0, 0, 1,  0, 0, 1,  0, 0, 1,  0, 0, 1,
             0, 0,-1,  0, 0,-1,  0, 0,-1,  0, 0,-1,
             0, 1, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0,
             0,-1, 0,  0,-1, 0,  0,-1, 0,  0,-1, 0,
             1, 0, 0,  1, 0, 0,  1, 0, 0,  1, 0, 0,
            -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0
        };

        const unsigned char indices[] = {
             0, 1, 2,   0, 2, 3,
             4, 5, 6,   4, 6, 7,
             8, 9, 10,  8, 10,11,
            12,13,14,  12,14,15,
            16,17,18,  16,18,19,
            20,21,22,  20,22,23
        };

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertices);

        if (!isShadow) {
            GLfloat mat_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
            glMaterialf(GL_FRONT, GL_SHININESS, 64.0f);

            glEnableClientState(GL_COLOR_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);
            glColorPointer(3, GL_FLOAT, 0, colors);
            glNormalPointer(GL_FLOAT, 0, normals);
        } else {
            glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
        }

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);
        glDisableClientState(GL_VERTEX_ARRAY);
        if (!isShadow) { glDisableClientState(GL_COLOR_ARRAY); glDisableClientState(GL_NORMAL_ARRAY); }

    } else {
        if (!isShadow) {
            GLfloat mat_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
            glMaterialf(GL_FRONT, GL_SHININESS, 64.0f);

            glEnable(GL_TEXTURE_2D);
            glColor3f(1.0f, 1.0f, 1.0f);
            glBindTexture(GL_TEXTURE_2D, textureId);
        } else {
            glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
        }

        glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, 1.0f);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
            if (!isShadow) glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
            if (!isShadow) glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
        glEnd();

        glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, -1.0f);
            if (!isShadow) glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
            if (!isShadow) glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
        glEnd();

        if (!isShadow && textureTopId != 0) glBindTexture(GL_TEXTURE_2D, textureTopId);
        glBegin(GL_QUADS);
            glNormal3f(0.0f, 1.0f, 0.0f);
            if (!isShadow) glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
            if (!isShadow) glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
        glEnd();
        if (!isShadow && textureTopId != 0) glBindTexture(GL_TEXTURE_2D, textureId);

        glBegin(GL_QUADS);
            glNormal3f(0.0f, -1.0f, 0.0f);
            if (!isShadow) glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
            if (!isShadow) glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
        glEnd();

        glBegin(GL_QUADS);
            glNormal3f(1.0f, 0.0f, 0.0f);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
            if (!isShadow) glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
            if (!isShadow) glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
        glEnd();

        glBegin(GL_QUADS);
            glNormal3f(-1.0f, 0.0f, 0.0f);
            if (!isShadow) glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
            if (!isShadow) glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
            if (!isShadow) glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
            if (!isShadow) glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
        glEnd();

        if (!isShadow) glDisable(GL_TEXTURE_2D);
    }
}

void PyramidObject::draw(bool isShadow) {
    const float vertices[] = {
         0.0f,  0.6f,  0.0f, -0.5f, -0.4f,  0.5f,  0.5f, -0.4f,  0.5f,  0.5f, -0.4f, -0.5f, -0.5f, -0.4f, -0.5f
    };
    const float colors[] = {
        1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, 0.0f
    };
    const float normals[] = {
         0.0f,  1.0f,  0.0f,  -0.5f,  0.5f,  0.5f,   0.5f,  0.5f,  0.5f,   0.5f,  0.5f, -0.5f,  -0.5f,  0.5f, -0.5f
    };
    const unsigned char indices[] = {
        0, 1, 2,  0, 2, 3,  0, 3, 4,  0, 4, 1,
        1, 4, 3,  1, 3, 2
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    if (!isShadow) {
        GLfloat mat_specular[] = { 0.6f, 0.6f, 0.6f, 1.0f };
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, 32.0f);

        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glColorPointer(3, GL_FLOAT, 0, colors);
        glNormalPointer(GL_FLOAT, 0, normals);
    } else {
        glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
    }

    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_BYTE, indices);
    glDisableClientState(GL_VERTEX_ARRAY);
    if (!isShadow) { glDisableClientState(GL_COLOR_ARRAY); glDisableClientState(GL_NORMAL_ARRAY); }
}

SphereObject::SphereObject(float x, float y, float z) : Object3D(x, y, z) {
    int sectors = 24; int stacks = 24; float radius = 0.5f;

    for (int i = 0; i < stacks; ++i) {
        float phi1 = M_PI * (float)i / stacks;
        float phi2 = M_PI * (float)(i + 1) / stacks;

        for (int j = 0; j < sectors; ++j) {
            float theta1 = 2.0f * M_PI * (float)j / sectors;
            float theta2 = 2.0f * M_PI * (float)(j + 1) / sectors;

            float p[4][3] = {
                { radius * sin(phi1) * cos(theta1), radius * cos(phi1), radius * sin(phi1) * sin(theta1) },
                { radius * sin(phi1) * cos(theta2), radius * cos(phi1), radius * sin(phi1) * sin(theta2) },
                { radius * sin(phi2) * cos(theta2), radius * cos(phi2), radius * sin(phi2) * sin(theta2) },
                { radius * sin(phi2) * cos(theta1), radius * cos(phi2), radius * sin(phi2) * sin(theta1) }
            };

            for (int k : {0, 1, 2, 0, 2, 3}) {
                vertices.push_back(p[k][0]); vertices.push_back(p[k][1]); vertices.push_back(p[k][2]);
                float len = sqrt(p[k][0]*p[k][0] + p[k][1]*p[k][1] + p[k][2]*p[k][2]);
                normals.push_back(p[k][0]/len); normals.push_back(p[k][1]/len); normals.push_back(p[k][2]/len);
            }
        }
    }
}

void SphereObject::draw(bool isShadow) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices.data());

    if (!isShadow) {
        GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, 64.0f);
        glColor3f(0.0f, 0.7f, 0.9f);

        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, normals.data());
    } else {
        glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
    }

    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
    glDisableClientState(GL_VERTEX_ARRAY);
    if (!isShadow) glDisableClientState(GL_NORMAL_ARRAY);
}

ConeObject::ConeObject(float x, float y, float z) : Object3D(x, y, z) {
    int sectors = 24; float radius = 0.5f; float height = 1.0f;

    for (int j = 0; j < sectors; ++j) {
        float theta1 = 2.0f * M_PI * (float)j / sectors;
        float theta2 = 2.0f * M_PI * (float)(j + 1) / sectors;

        vertices.insert(vertices.end(), { 0.0f, 0.0f, 0.0f, radius * cos(theta2), 0.0f, radius * sin(theta2), radius * cos(theta1), 0.0f, radius * sin(theta1) });
        normals.insert(normals.end(), { 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f });

        float nx = cos((theta1 + theta2) / 2.0f); float ny = radius / height; float nz = sin((theta1 + theta2) / 2.0f);
        float len = sqrt(nx*nx + ny*ny + nz*nz);
        nx /= len; ny /= len; nz /= len;

        vertices.insert(vertices.end(), { 0.0f, height, 0.0f, radius * cos(theta1), 0.0f, radius * sin(theta1), radius * cos(theta2), 0.0f, radius * sin(theta2) });
        normals.insert(normals.end(), { nx, ny, nz, nx, ny, nz, nx, ny, nz });
    }
}

void ConeObject::draw(bool isShadow) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices.data());

    if (!isShadow) {
        GLfloat mat_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, 48.0f);
        glColor3f(0.9f, 0.5f, 0.0f);

        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, normals.data());
    } else {
        glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
    }

    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
    glDisableClientState(GL_VERTEX_ARRAY);
    if (!isShadow) glDisableClientState(GL_NORMAL_ARRAY);
}

void FloorObject::draw(bool isShadow) {
    if (isShadow) return;

    GLfloat mat_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 8.0f);

    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);

    for (int i = -10; i < 10; i++) {
        for (int j = -10; j < 10; j++) {
            glTexCoord2f(0.0f, 0.0f); glVertex3f((float)i,   0.0f, (float)j);
            glTexCoord2f(1.0f, 0.0f); glVertex3f((float)i+1, 0.0f, (float)j);
            glTexCoord2f(1.0f, 1.0f); glVertex3f((float)i+1, 0.0f, (float)j+1);
            glTexCoord2f(0.0f, 1.0f); glVertex3f((float)i,   0.0f, (float)j+1);
        }
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

std::vector<Object3D*> GeometryProvider::initScene() { return std::vector<Object3D*>(); }