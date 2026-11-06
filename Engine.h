/**
 * @file Engine.h
 * @brief Główna klasa zarządzająca silnikiem gry 3D opartym na bibliotece FreeGLUT.
 * @author Twój Projekt
 * @date 2026
 */

#ifndef ENGINE_H
#define ENGINE_H

#include <GL/freeglut.h>
#include <vector>
#include <string>
#include "GeometryProvider.h"

class Camera;
class Player;

/**
 * @struct SurfaceInfo
 * @brief Przechowuje informacje o aktualnym podłożu pod graczem (wysokość, czy to lód lub lodowa rampa).
 */
struct SurfaceInfo {
    float floorY = 0.0f;
    bool isIce = false;
    bool isIceSlope = false;
};

/**
 * @class Engine
 * @brief Służy jako centralny kontroler podsystemów renderowania, wejścia i prostej fizyki gier.
 */
class Engine {
private:
    int windowWidth;       ///< Szerokość okna aplikacji w pikselach.
    int windowHeight;      ///< Wysokość okna aplikacji w pikselach.
    int fps;               ///< Aktualny docelowy limit klatek na sekundę (FPS).
    bool isPerspective;    ///< Flaga określająca stan rzutowania (true = perspektywa, false = ortogonalne).
    bool isPaused;         ///< Stan zatrzymania symulacji gry.

    bool isLightingEnabled;  ///< Flaga włączająca podstawowe oświetlenie sceny.
    bool areShadowsEnabled;   ///< Flaga kontrolująca generowanie płaskich cieni na podłożu.
    bool isSmoothShading;    ///< Flaga wyboru modelu cieniowania (Gouraud vs Flat).

    bool keyStates[256];     ///< Tablica asynchronicznych stanów klawiszy klawiatury.

    std::vector<Object3D*> sceneObjects; ///< Kontener przechowujący wskaźniki do wszystkich obiektów sceny.
    Camera* camera;          ///< Wskaźnik na instancję kamery TPP.
    Player* player;          ///< Wskaźnik na postać gracza.

    /**
     * @brief Przetwarza ruch gracza w oparciu o stan wciśniętych klawiszy i orientację kamery.
     */
    void processMovement();

    /**
     * @brief Wykrywa kolizje typu AABB (3D) pomiędzy graczem a elementami otoczenia.
     */
    bool checkCollision3D(float px, float py, float pz, float sx, float sy, float sz, Object3D* ignoreObj);

    /**
     * @brief Oblicza najwyższy dostępny poziom podłoża pod podanymi współrzędnymi.
     */
    float getFloorLevelAt(float x, float z);

    /**
     * @brief Zwraca szczegółowe informacje o właściwościach podłoża (lód, pochyłość, wysokość).
     */
    SurfaceInfo getSurfaceInfoAt(float x, float z);

    /**
     * @brief Renderuje tekst rastrowy 2D na ekranie.
     */
    void renderText(float x, float y, const std::string& text);

    /**
     * @brief Tworzy macierz rzutowania płaskiego cienia na podłoże Y=0.
     */
    void buildShadowMatrix(float shadowMat[16], float lightPos[4]);

public:
    static Engine* instance; ///< Statyczny wskaźnik na unikalną instancję silnika (Wzorzec Singleton).

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

    /* --- Wrappery statyczne wymagane przez interfejs C-style biblioteki GLUT --- */
    static void displayWrapper();
    static void reshapeWrapper(int w, int h);
    static void timerWrapper(int value);
    static void keyDownWrapper(unsigned char key, int x, int y);
    static void keyUpWrapper(unsigned char key, int x, int y);
    static void passiveMotionWrapper(int x, int y);
};

#endif