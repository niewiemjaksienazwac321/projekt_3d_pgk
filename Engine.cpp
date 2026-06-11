/**
 * @file Engine.cpp
 * @brief Implementacja metod klasy Engine oraz obsługi stanów gry i menu interaktywnego.
 */

#include "Engine.h"
#include "Camera.h"
#include "Player.h"
#include "BitmapHandler.h"
#include <iostream>
#include <cmath>
#include <algorithm>

Engine* Engine::instance = nullptr;

Engine::Engine() {
    windowWidth = 1280;
    windowHeight = 720;
    fps = 60; ///< Domyślna wartość płynności renderowania
    isPerspective = true;
    isPaused = false;

    isLightingEnabled = true;
    areShadowsEnabled = true;
    isSmoothShading = true;

    for (int i = 0; i < 256; i++) {
        keyStates[i] = false;
    }

    camera = nullptr;
    player = nullptr;
}

void Engine::init(int* argc, char** argv) {
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Silnik 3D - Prawdziwe Cienie i Oswietlenie");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE);
    glShadeModel(isSmoothShading ? GL_SMOOTH : GL_FLAT);

    glutDisplayFunc(displayWrapper);
    glutReshapeFunc(reshapeWrapper);
    glutTimerFunc(1000 / fps, timerWrapper, 0);
    glutKeyboardFunc(keyDownWrapper);
    glutKeyboardUpFunc(keyUpWrapper);
    glutPassiveMotionFunc(passiveMotionWrapper);
    glutSetCursor(GLUT_CURSOR_NONE);

    camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f));

    // Ładowanie tekstur z plików BMP/PNG
    GLuint texGrass  = BitmapHandler::loadTexture("trawa.png");
    GLuint texPFront = BitmapHandler::loadTexture("gracz_przod.png");
    GLuint texPBack  = BitmapHandler::loadTexture("gracz_tyl.png");
    GLuint texPLeft  = BitmapHandler::loadTexture("gracz_lewo.png");
    GLuint texPRight = BitmapHandler::loadTexture("gracz_prawo.png");
    GLuint texPTop   = BitmapHandler::loadTexture("gracz_gora.png");

    FloorObject* floor = new FloorObject();
    floor->textureId = texGrass;
    sceneObjects.push_back(floor);

    player = new Player(0.0f, 0.0f, 0.0f);
    player->setTextures(texPFront, texPBack, texPLeft, texPRight, texPTop);
    sceneObjects.push_back(player);

    CubeObject* box1 = new CubeObject(-2.0f, 0.5f, -2.0f);
    box1->scaleX = 1.5f; box1->scaleY = 1.0f; box1->scaleZ = 1.5f;
    sceneObjects.push_back(box1);

    CubeObject* box2 = new CubeObject(-2.0f, 1.0f, -4.5f);
    box2->scaleX = 1.5f; box2->scaleY = 2.0f; box2->scaleZ = 1.5f;
    sceneObjects.push_back(box2);

    SphereObject* sphere = new SphereObject(3.0f, 0.5f, 2.0f);
    sceneObjects.push_back(sphere);

    ConeObject* cone = new ConeObject(-4.0f, 0.0f, 2.0f);
    sceneObjects.push_back(cone);

    PyramidObject* pyramid = new PyramidObject(6.0f, 0.6f, -4.0f);
    pyramid->scaleX = 1.5f; pyramid->scaleY = 1.5f; pyramid->scaleZ = 1.5f;
    sceneObjects.push_back(pyramid);

    TriangleObject* triangle = new TriangleObject(0.0f, 1.0f, -4.0f);
    sceneObjects.push_back(triangle);
}

void Engine::start() { glutMainLoop(); }

void Engine::renderText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void Engine::buildShadowMatrix(float shadowMat[16], float lightPos[4]) {
    float dot = lightPos[1];
    shadowMat[0] = dot; shadowMat[4] = -lightPos[0]; shadowMat[8]  = 0.0f; shadowMat[12] = 0.0f;
    shadowMat[1] = 0.0f; shadowMat[5] = 0.0f;        shadowMat[9]  = 0.0f; shadowMat[13] = 0.0f;
    shadowMat[2] = 0.0f; shadowMat[6] = -lightPos[2]; shadowMat[10] = dot; shadowMat[14] = 0.0f;
    shadowMat[3] = 0.0f; shadowMat[7] = -lightPos[3]; shadowMat[11] = 0.0f; shadowMat[15] = dot;
}

bool Engine::checkCollision3D(float px, float py, float pz, float sx, float sy, float sz, Object3D* ignoreObj) {
    float pMinX = px - sx / 2.0f; float pMaxX = px + sx / 2.0f;
    float pMinY = py - sy / 2.0f; float pMaxY = py + sy / 2.0f;
    float pMinZ = pz - sz / 2.0f; float pMaxZ = pz + sz / 2.0f;

    for (auto* obj : sceneObjects) {
        if (obj == ignoreObj || dynamic_cast<FloorObject*>(obj) != nullptr) continue;

        float oMinX = obj->posX - obj->scaleX / 2.0f; float oMaxX = obj->posX + obj->scaleX / 2.0f;
        float oMinY = obj->posY - obj->scaleY / 2.0f; float oMaxY = obj->posY + obj->scaleY / 2.0f;
        float oMinZ = obj->posZ - obj->scaleZ / 2.0f; float oMaxZ = obj->posZ + obj->scaleZ / 2.0f;

        if (pMaxX > oMinX && pMinX < oMaxX && pMaxY > oMinY && pMinY < oMaxY && pMaxZ > oMinZ && pMinZ < oMaxZ) {
            return true;
        }
    }
    return false;
}

float Engine::getFloorLevelAt(float x, float z) {
    float highestFloor = 0.0f;
    float pRadius = player->scaleX / 2.0f;

    for (auto* obj : sceneObjects) {
        if (obj == player || dynamic_cast<FloorObject*>(obj) != nullptr) continue;

        float oMinX = obj->posX - obj->scaleX / 2.0f; float oMaxX = obj->posX + obj->scaleX / 2.0f;
        float oMinZ = obj->posZ - obj->scaleZ / 2.0f; float oMaxZ = obj->posZ + obj->scaleZ / 2.0f;

        if (x + pRadius > oMinX && x - pRadius < oMaxX && z + pRadius > oMinZ && z - pRadius < oMaxZ) {
            float roofY = obj->posY + (obj->scaleY / 2.0f);
            if (player->posY >= roofY - 0.2f) {
                if (roofY > highestFloor) highestFloor = roofY;
            }
        }
    }
    return highestFloor;
}

void Engine::processMovement() {
    if (isPaused || !player || !camera) return;

    float speed = 0.12f;
    float radYaw = glm::radians(camera->yaw);

    glm::vec3 camForward = glm::normalize(glm::vec3(cos(radYaw), 0.0f, sin(radYaw)));
    glm::vec3 camRight = glm::normalize(glm::cross(camForward, glm::vec3(0.0f, 1.0f, 0.0f)));

    float moveX = 0.0f, moveZ = 0.0f;

    if (keyStates['w'] || keyStates['W']) { moveX -= camForward.x; moveZ -= camForward.z; }
    if (keyStates['s'] || keyStates['S']) { moveX += camForward.x; moveZ += camForward.z; }
    if (keyStates['a'] || keyStates['A']) { moveX += camRight.x;  moveZ += camRight.z; }
    if (keyStates['d'] || keyStates['D']) { moveX -= camRight.x;  moveZ -= camRight.z; }

    if (moveX != 0.0f || moveZ != 0.0f) {
        glm::vec3 moveDir = glm::normalize(glm::vec3(moveX, 0.0f, moveZ)) * speed;
        player->rotY = glm::degrees(std::atan2(moveDir.x, moveDir.z));

        float nextX = player->posX + moveDir.x;
        float nextZ = player->posZ + moveDir.z;
        float boundary = 10.0f - (player->scaleX * 0.5f);

        if (nextX > -boundary && nextX < boundary) {
            if (!checkCollision3D(nextX, player->posY, player->posZ, player->scaleX, player->scaleY, player->scaleZ, player)) {
                player->posX = nextX;
            }
        }
        if (nextZ > -boundary && nextZ < boundary) {
            if (!checkCollision3D(player->posX, player->posY, nextZ, player->scaleX, player->scaleY, player->scaleZ, player)) {
                player->posZ = nextZ;
            }
        }
    }

    if (keyStates[' ']) player->jump();
}

void Engine::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLfloat lightPosition[] = { 6.0f, 20.0f, 4.0f, 2.0f };

    if (isLightingEnabled) {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        GLfloat lightAmbient[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
        GLfloat lightDiffuse[]  = { 0.7f, 0.7f, 0.7f, 1.0f };
        GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    } else {
        glDisable(GL_LIGHTING);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (camera && player) {
        camera->updateTPP(player->posX, player->posY, player->posZ);
        camera->apply();
    }

    for (auto* obj : sceneObjects) {
        glPushMatrix();
        obj->applyTransformations();
        obj->draw();
        glPopMatrix();
    }

    if (areShadowsEnabled) {
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);

        glPushMatrix();
        glTranslatef(0.0f, 0.01f, 0.0f);

        float shadowMat[16];
        buildShadowMatrix(shadowMat, lightPosition);
        glMultMatrixf(shadowMat);

        for (auto* obj : sceneObjects) {
            if (dynamic_cast<FloorObject*>(obj) != nullptr) continue;

            glPushMatrix();
            obj->applyTransformations();
            obj->draw(true);
            glPopMatrix();
        }

        glPopMatrix();

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }

    if (isPaused) {
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);

        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, windowWidth, 0, windowHeight);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f, 0.0f, 0.0f, 0.75f);
        glBegin(GL_QUADS);
            glVertex2f(0.0f, 0.0f); glVertex2f((float)windowWidth, 0.0f);
            glVertex2f((float)windowWidth, (float)windowHeight); glVertex2f(0.0f, (float)windowHeight);
        glEnd();
        glDisable(GL_BLEND);

        glColor3f(1.0f, 1.0f, 1.0f);
        renderText(windowWidth / 2.0f - 80.0f, windowHeight - 150.0f, "GRA PAUZOWANA");

        renderText(windowWidth / 2.0f - 180.0f, windowHeight - 250.0f, isLightingEnabled ? "[1] REJ TREJSING -> ON" : "[1] REJ TREJSING -> OFF");
        renderText(windowWidth / 2.0f - 180.0f, windowHeight - 300.0f, areShadowsEnabled ? "[2] Cienie rzutowane -> AKTYWNE" : "[2] Cienie rzutowane -> WYLACZONE");
        renderText(windowWidth / 2.0f - 180.0f, windowHeight - 350.0f, isSmoothShading ? "[3] PATH TREJSING -> ON" : "[3] PATH TREJSING -> OFF");
        renderText(windowWidth / 2.0f - 180.0f, windowHeight - 400.0f, isPerspective ? "[4] Rzutowanie -> PERSPEKTYWA" : "[4] Rzutowanie -> ORTOGONALNE");

        // NOWA OPCJA UI: dynamiczne wyświetlanie klatek z oznaczeniem trybu kinowego
        renderText(windowWidth / 2.0f - 180.0f, windowHeight - 450.0f, "[5] Limit FPS -> " + std::to_string(fps) + (fps == 15 ? " (Tryb Konsolowy)" : ""));

        glColor3f(0.8f, 0.8f, 0.8f);
        renderText(windowWidth / 2.0f - 130.0f, windowHeight - 520.0f, "Nacisnij 'P', aby wznowic gre");

        glEnable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }

    glutSwapBuffers();
}

void Engine::update(int value) {
    processMovement();
    if (player) player->updatePhysics(getFloorLevelAt(player->posX, player->posZ));
    glutPostRedisplay();

    // Elastyczne wywołanie pętli czasowej na podstawie zmiennej fps
    glutTimerFunc(1000 / fps, timerWrapper, 0);
}

void Engine::handleKeyDown(unsigned char key, int x, int y) {
    if (isPaused) {
        if (key == '1') { isLightingEnabled = !isLightingEnabled; glutPostRedisplay(); return; }
        if (key == '2') { areShadowsEnabled = !areShadowsEnabled; glutPostRedisplay(); return; }
        if (key == '3') {
            isSmoothShading = !isSmoothShading;
            glShadeModel(isSmoothShading ? GL_SMOOTH : GL_FLAT);
            glutPostRedisplay();
            return;
        }
        if (key == '4') {
            isPerspective = !isPerspective;
            reshape(windowWidth, windowHeight);
            glutPostRedisplay();
            return;
        }
        // NOWOŚĆ: Przełączanie wartości FPS (30 -> 60 -> 144 -> 15 -> powrót do 30)
        if (key == '5') {
            if (fps == 30) fps = 60;
            else if (fps == 60) fps = 144;
            else if (fps == 144) fps = 15;
            else fps = 30;
            glutPostRedisplay();
            return;
        }
    }

    keyStates[key] = true;
    if (key == 27) { deinit(); exit(0); }
    if (key == 'p' || key == 'P') {
        isPaused = !isPaused;
        if (isPaused) { glutSetCursor(GLUT_CURSOR_INFO); }
        else { glutSetCursor(GLUT_CURSOR_NONE); glutWarpPointer(windowWidth / 2, windowHeight / 2); }
    }
}

void Engine::handleKeyUp(unsigned char key, int x, int y) { keyStates[key] = false; }

void Engine::handlePassiveMotion(int x, int y) {
    if (isPaused || !camera) return;
    int centerX = windowWidth / 2, centerY = windowHeight / 2;
    if (x == centerX && y == centerY) return;
    camera->processMouseMovement(x - centerX, centerY - y);
    glutWarpPointer(centerX, centerY);
    glutPostRedisplay();
}

void Engine::reshape(int w, int h) {
    windowWidth = w; windowHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (isPerspective) {
        gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    } else {
        double aspect = (double)w / (double)h;
        glOrtho(-10.0 * aspect, 10.0 * aspect, -10.0, 10.0, -20.0, 100.0);
    }

    glMatrixMode(GL_MODELVIEW);
}

void Engine::deinit() {
    std::cout << "Zamykanie...\n";
    for (auto* obj : sceneObjects) delete obj;
    sceneObjects.clear();
    if (camera) delete camera;
}

void Engine::displayWrapper() { if (instance) instance->render(); }
void Engine::reshapeWrapper(int w, int h) { if (instance) instance->reshape(w, h); }
void Engine::timerWrapper(int value) { if (instance) instance->update(value); }
void Engine::keyDownWrapper(unsigned char key, int x, int y) { if (instance) instance->handleKeyDown(key, x, y); }
void Engine::keyUpWrapper(unsigned char key, int x, int y) { if (instance) instance->handleKeyUp(key, x, y); }
void Engine::passiveMotionWrapper(int x, int y) { if (instance) instance->handlePassiveMotion(x, y); }