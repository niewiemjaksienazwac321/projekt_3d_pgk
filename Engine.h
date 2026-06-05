#ifndef ENGINE_H
#define ENGINE_H

#include <GL/freeglut.h>
#include <vector>
#include <string>
#include "GeometryProvider.h"

class Camera;
class Player;

class Engine {
private:
    int windowWidth, windowHeight;
    int fps;
    bool isPerspective;
    bool isPaused;

    bool isLightingEnabled;
    bool areShadowsEnabled;
    bool isSmoothShading;

    bool keyStates[256];

    std::vector<Object3D*> sceneObjects;
    Camera* camera;
    Player* player;

    void processMovement();
    bool checkCollision3D(float px, float py, float pz, float sx, float sy, float sz, Object3D* ignoreObj);
    float getFloorLevelAt(float x, float z);

    // Rysowanie UI
    void renderText(float x, float y, const std::string& text);

    // NOWE: Funkcja matematyczna generująca macierz rzutu płaskiego na podłogę
    void buildShadowMatrix(float shadowMat[16], float lightPos[4]);

public:
    static Engine* instance;

    Engine();
    void init(int* argc, char** argv);
    void start();
    void deinit();

    void render();
    void reshape(int w, int h);
    void update(int value);

    void handleKeyDown(unsigned char key, int x, int y);
    void handleKeyUp(unsigned char key, int x, int y);
    void handlePassiveMotion(int x, int y);

    static void displayWrapper();
    static void reshapeWrapper(int w, int h);
    static void timerWrapper(int value);
    static void keyDownWrapper(unsigned char key, int x, int y);
    static void keyUpWrapper(unsigned char key, int x, int y);
    static void passiveMotionWrapper(int x, int y);
};

#endif