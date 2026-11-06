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
    fps = 60;
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

    // --- Ładowanie tekstur w formacie .png ---
    GLuint texGrass    = BitmapHandler::loadTexture("trawa.png");
    GLuint texNoga     = BitmapHandler::loadTexture("noga.png");
    GLuint texReka     = BitmapHandler::loadTexture("reka.png");
    GLuint texLawa     = BitmapHandler::loadTexture("lawa.png");
    GLuint texGlowaF   = BitmapHandler::loadTexture("glowa_przod.png");
    GLuint texGlowaT   = BitmapHandler::loadTexture("glowa_tyl.png");
    GLuint texLod      = BitmapHandler::loadTexture("lod.png");

    FloorObject* floor = new FloorObject();
    floor->textureId = texGrass;
    sceneObjects.push_back(floor);

    // --- Inicjalizacja gracza z teksturami kończyn, tułowia i głowy ---
    player = new Player(0.0f, 0.0f, 0.0f);
    player->setTextures(texNoga, texReka, texGrass, texGrass, texGrass, texGrass, texGrass, texGlowaF, texGlowaT);
    player->head.textureId = texGlowaF; // Przypisanie tekstury glowa_przod.png do głowy
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

    // --- Płaska platforma z lodu (lod.png) ---
    IcePlatformObject* icePlat = new IcePlatformObject(0.0f, 0.5f, 4.0f);
    icePlat->scaleX = 3.0f; icePlat->scaleY = 0.3f; icePlat->scaleZ = 3.0f;
    icePlat->textureId = texLod;
    sceneObjects.push_back(icePlat);

    // --- Pochyła rampa z lodu (lod.png) ---
    IceSlopeObject* iceSlope = new IceSlopeObject(4.0f, 0.0f, 0.0f);
    iceSlope->scaleX = 2.5f; iceSlope->scaleY = 2.0f; iceSlope->scaleZ = 3.0f;
    iceSlope->textureId = texLod;
    sceneObjects.push_back(iceSlope);

    // --- Fragment gry: Basen z lawą (lawa.png) ---
    LavaObject* lavaPool = new LavaObject(0.0f, -0.05f, -8.0f);
    lavaPool->scaleX = 6.0f; lavaPool->scaleY = 0.2f; lavaPool->scaleZ = 4.0f;
    lavaPool->textureId = texLawa;
    sceneObjects.push_back(lavaPool);

    // --- Ruchoma platforma pionowa (góra-dół) ---
    MovingPlatformObject* movPlatformV = new MovingPlatformObject(
        glm::vec3(3.0f, 0.5f, -2.0f),
        glm::vec3(3.0f, 3.5f, -2.0f),
        0.015f
    );
    movPlatformV->scaleX = 2.0f; movPlatformV->scaleY = 0.3f; movPlatformV->scaleZ = 2.0f;
    movPlatformV->textureId = texGrass;
    sceneObjects.push_back(movPlatformV);

    // --- Ruchoma platforma pozioma (lewo-prawo) ---
    MovingPlatformObject* movPlatformH = new MovingPlatformObject(
        glm::vec3(-4.0f, 1.0f, 2.0f),
        glm::vec3(1.0f, 1.0f, 2.0f),
        0.015f
    );
    movPlatformH->scaleX = 2.0f; movPlatformH->scaleY = 0.3f; movPlatformH->scaleZ = 2.0f;
    movPlatformH->textureId = texGrass;
    sceneObjects.push_back(movPlatformH);

    // --- Platforma pochyła (Rampa zwykła) ---
    SlopePlatformObject* slope = new SlopePlatformObject(-4.0f, 0.0f, -2.0f);
    slope->scaleX = 2.0f; slope->scaleY = 2.0f; slope->scaleZ = 3.0f;
    sceneObjects.push_back(slope);
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
        // Ignorujemy podłogę oraz platformy pochyłe
        if (obj == ignoreObj || dynamic_cast<FloorObject*>(obj) != nullptr ||
            dynamic_cast<SlopePlatformObject*>(obj) != nullptr) continue;

        float oMinX = obj->posX - obj->scaleX / 2.0f; float oMaxX = obj->posX + obj->scaleX / 2.0f;
        float oMinY = obj->posY - obj->scaleY / 2.0f; float oMaxY = obj->posY + obj->scaleY / 2.0f;
        float oMinZ = obj->posZ - obj->scaleZ / 2.0f; float oMaxZ = obj->posZ + obj->scaleZ / 2.0f;

        // Jeśli gracz stoi na górze obiektu, nie blokujemy ruchu poziomego
        if (pMinY >= oMaxY - 0.1f) {
            continue;
        }

        if (pMaxX > oMinX && pMinX < oMaxX && pMaxY > oMinY && pMinY < oMaxY && pMaxZ > oMinZ && pMinZ < oMaxZ) {
            return true;
        }
    }
    return false;
}

SurfaceInfo Engine::getSurfaceInfoAt(float x, float z) {
    SurfaceInfo info;
    float highestFloor = 0.0f;
    float pRadius = player->scaleX / 2.0f;
    float playerBottom = player->posY - (player->scaleY / 2.0f); // Poziom stóp gracza

    for (auto* obj : sceneObjects) {
        if (obj == player || dynamic_cast<FloorObject*>(obj) != nullptr) continue;

        // Lodowa rampa pochyła
        IceSlopeObject* iceSlope = dynamic_cast<IceSlopeObject*>(obj);
        if (iceSlope != nullptr) {
            float localX = x - iceSlope->posX;
            float localZ = z - iceSlope->posZ;
            float minX = -iceSlope->scaleX / 2.0f; float maxX = iceSlope->scaleX / 2.0f;
            float minZ = -iceSlope->scaleZ / 2.0f; float maxZ = iceSlope->scaleZ / 2.0f;

            if (localX + pRadius > minX && localX - pRadius < maxX && localZ + pRadius > minZ && localZ - pRadius < maxZ) {
                float normalizedLocalZ = localZ / iceSlope->scaleZ;
                float roofY = iceSlope->posY + (normalizedLocalZ + 0.5f) * iceSlope->scaleY;

                if (player->posY >= roofY - 0.3f && roofY >= highestFloor) {
                    highestFloor = roofY;
                    info.floorY = roofY;

                    // Efekt lodu działa TYLKO wtedy, gdy stopy gracza są faktycznie na powierzchni rampy
                    if (std::abs(playerBottom - roofY) < 0.35f) {
                        info.isIce = true;
                        info.isIceSlope = true;
                    } else {
                        info.isIce = false;
                        info.isIceSlope = false;
                    }
                }
            }
            continue;
        }

        // Płaska platforma z lodu
        IcePlatformObject* icePlat = dynamic_cast<IcePlatformObject*>(obj);
        if (icePlat != nullptr) {
            float oMinX = icePlat->posX - icePlat->scaleX / 2.0f; float oMaxX = icePlat->posX + icePlat->scaleX / 2.0f;
            float oMinZ = icePlat->posZ - icePlat->scaleZ / 2.0f; float oMaxZ = icePlat->posZ + icePlat->scaleZ / 2.0f;

            if (x + pRadius > oMinX && x - pRadius < oMaxX && z + pRadius > oMinZ && z - pRadius < oMaxZ) {
                float roofY = icePlat->posY + (icePlat->scaleY / 2.0f);
                if (player->posY >= roofY - 0.2f && roofY >= highestFloor) {
                    highestFloor = roofY;
                    info.floorY = roofY;

                    if (std::abs(playerBottom - roofY) < 0.35f) {
                        info.isIce = true;
                        info.isIceSlope = false;
                    } else {
                        info.isIce = false;
                        info.isIceSlope = false;
                    }
                }
            }
            continue;
        }

        // Zwykła rampa pochyła
        SlopePlatformObject* slope = dynamic_cast<SlopePlatformObject*>(obj);
        if (slope != nullptr) {
            float localX = x - slope->posX;
            float localZ = z - slope->posZ;
            float minX = -slope->scaleX / 2.0f; float maxX = slope->scaleX / 2.0f;
            float minZ = -slope->scaleZ / 2.0f; float maxZ = slope->scaleZ / 2.0f;

            if (localX + pRadius > minX && localX - pRadius < maxX && localZ + pRadius > minZ && localZ - pRadius < maxZ) {
                float normalizedLocalZ = localZ / slope->scaleZ;
                float roofY = slope->posY + (normalizedLocalZ + 0.5f) * slope->scaleY;
                if (player->posY >= roofY - 0.3f && roofY >= highestFloor) {
                    highestFloor = roofY;
                    info.floorY = roofY;
                    info.isIce = false;
                    info.isIceSlope = false;
                }
            }
            continue;
        }

        // Standardowe sześciany / platformy
        float oMinX = obj->posX - obj->scaleX / 2.0f; float oMaxX = obj->posX + obj->scaleX / 2.0f;
        float oMinZ = obj->posZ - obj->scaleZ / 2.0f; float oMaxZ = obj->posZ + obj->scaleZ / 2.0f;

        if (x + pRadius > oMinX && x - pRadius < oMaxX && z + pRadius > oMinZ && z - pRadius < oMaxZ) {
            float roofY = obj->posY + (obj->scaleY / 2.0f);
            if (player->posY >= roofY - 0.2f && roofY >= highestFloor) {
                highestFloor = roofY;
                info.floorY = roofY;
                info.isIce = false;
                info.isIceSlope = false;
            }
        }
    }
    return info;
}

float Engine::getFloorLevelAt(float x, float z) {
    return getSurfaceInfoAt(x, z).floorY;
}

void Engine::processMovement() {
    if (isPaused || !player || !camera) return;

    float radYaw = glm::radians(camera->yaw);
    glm::vec3 camForward = glm::normalize(glm::vec3(cos(radYaw), 0.0f, sin(radYaw)));
    glm::vec3 camRight = glm::normalize(glm::cross(camForward, glm::vec3(0.0f, 1.0f, 0.0f)));

    float moveX = 0.0f, moveZ = 0.0f;
    bool isMoving = false;

    if (keyStates['w'] || keyStates['W']) { moveX -= camForward.x; moveZ -= camForward.z; isMoving = true; }
    if (keyStates['s'] || keyStates['S']) { moveX += camForward.x; moveZ += camForward.z; isMoving = true; }
    if (keyStates['a'] || keyStates['A']) { moveX += camRight.x;  moveZ += camRight.z; isMoving = true; }
    if (keyStates['d'] || keyStates['D']) { moveX -= camRight.x;  moveZ -= camRight.z; isMoving = true; }

    glm::vec3 inputDir(0.0f);
    if (isMoving) {
        inputDir = glm::normalize(glm::vec3(moveX, 0.0f, moveZ));
    }

    SurfaceInfo surf = getSurfaceInfoAt(player->posX, player->posZ);
    player->updateMovementPhysics(isMoving, inputDir, surf.isIce, surf.isIceSlope);

    // Wywołanie bez niepotrzebnych flag
    player->updateAnimation();
    glm::vec3 activeVel = (glm::length(player->hVelocity) > 0.001f) ? player->hVelocity : inputDir;
    if (glm::length(activeVel) > 0.001f) {
        player->rotY = glm::degrees(std::atan2(activeVel.x, activeVel.z));
    }

    glm::vec3 vel = player->hVelocity;
    float nextX = player->posX + vel.x;
    float nextZ = player->posZ + vel.z;
    float boundary = 10.0f - (player->scaleX * 0.5f);

    if (nextX > -boundary && nextX < boundary) {
        if (!checkCollision3D(nextX, player->posY, player->posZ, player->scaleX, player->scaleY, player->scaleZ, player)) {
            player->posX = nextX;
        } else {
            player->hVelocity.x = 0.0f;
        }
    } else {
        player->hVelocity.x = 0.0f;
    }

    if (nextZ > -boundary && nextZ < boundary) {
        if (!checkCollision3D(player->posX, player->posY, nextZ, player->scaleX, player->scaleY, player->scaleZ, player)) {
            player->posZ = nextZ;
        } else {
            player->hVelocity.z = 0.0f;
        }
    } else {
        player->hVelocity.z = 0.0f;
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

    // Wyliczanie i aplikowanie przemieszczenia ruchomych platform oraz kolizji z lawą
    for (auto* obj : sceneObjects) {
        MovingPlatformObject* movPlat = dynamic_cast<MovingPlatformObject*>(obj);
        if (movPlat != nullptr) {
            glm::vec3 delta = movPlat->updatePosition();

            float pMinX = movPlat->posX - movPlat->scaleX / 2.0f;
            float pMaxX = movPlat->posX + movPlat->scaleX / 2.0f;
            float pMinZ = movPlat->posZ - movPlat->scaleZ / 2.0f;
            float pMaxZ = movPlat->posZ + movPlat->scaleZ / 2.0f;
            float platformSurfaceY = movPlat->posY + (movPlat->scaleY / 2.0f);

            if (player->posX >= pMinX && player->posX <= pMaxX &&
                player->posZ >= pMinZ && player->posZ <= pMaxZ &&
                std::abs(player->posY - (platformSurfaceY + player->scaleY / 2.0f)) < 0.15f) {
                player->posX += delta.x;
                player->posY += delta.y;
                player->posZ += delta.z;
            }
        }

        // --- Sprawdzanie kolizji z lawą (Reset do pozycji startowej) ---
        LavaObject* lava = dynamic_cast<LavaObject*>(obj);
        if (lava != nullptr && player != nullptr) {
            float lMinX = lava->posX - lava->scaleX / 2.0f; float lMaxX = lava->posX + lava->scaleX / 2.0f;
            float lMinY = lava->posY - lava->scaleY / 2.0f; float lMaxY = lava->posY + lava->scaleY / 2.0f;
            float lMinZ = lava->posZ - lava->scaleZ / 2.0f; float lMaxZ = lava->posZ + lava->scaleZ / 2.0f;

            float pMinX = player->posX - player->scaleX / 2.0f; float pMaxX = player->posX + player->scaleX / 2.0f;
            float pMinY = player->posY - player->scaleY / 2.0f; float pMaxY = player->posY + player->scaleY / 2.0f;
            float pMinZ = player->posZ - player->scaleZ / 2.0f; float pMaxZ = player->posZ + player->scaleZ / 2.0f;

            if (pMaxX > lMinX && pMinX < lMaxX && pMaxY > lMinY && pMinY < lMaxY && pMaxZ > lMinZ && pMinZ < lMaxZ) {
                // Cofnięcie gracza na start (0, 1, 0)
                player->posX = 0.0f;
                player->posY = 1.0f;
                player->posZ = 0.0f;
                player->velocityY = 0.0f;
                player->hVelocity = glm::vec3(0.0f);
            }
        }
    }

    if (player) player->updatePhysics(getFloorLevelAt(player->posX, player->posZ));
    glutPostRedisplay();

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