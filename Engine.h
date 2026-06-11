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
     * @param px Przyszła pozycja X.
     * @param py Przyszła pozycja Y.
     * @param pz Przyszła pozycja Z.
     * @param sx Skala (szerokość) obiektu testowanego.
     * @param sy Skala (wysokość) obiektu testowanego.
     * @param sz Skala (głębokość) obiektu testowanego.
     * @param ignoreObj Wskaźnik na obiekt ignorowany podczas testu (zwykle sam gracz).
     * @return true Jeśli występuje nakładanie obiektów, false w przeciwnym wypadku.
     */
    bool checkCollision3D(float px, float py, float pz, float sx, float sy, float sz, Object3D* ignoreObj);

    /**
     * @brief Oblicza najwyższy dostępny poziom podłoża (np. dachy pudełek) pod podanymi współrzędnymi.
     * @param x Współrzędna X świata.
     * @param z Współrzędna Z świata.
     * @return Najwyższy punkt Y powierzchni, na której można stanąć.
     */
    float getFloorLevelAt(float x, float z);

    /**
     * @brief Renderuje tekst rastrowy 2D na ekranie.
     * @param x Pozycja X na ekranie.
     * @param y Pozycja Y na ekranie.
     * @param text Ciąg znaków do wyświetlenia.
     */
    void renderText(float x, float y, const std::string& text);

    /**
     * @brief Tworzy macierz rzutowania płaskiego cienia na podłoże Y=0.
     * @param shadowMat Tablica 16-elementowa wyjściowej macierzy transformacji.
     * @param lightPos 4-elementowy wektor pozycji źródła światła.
     */
    void buildShadowMatrix(float shadowMat[16], float lightPos[4]);

public:
    static Engine* instance; ///< Statyczny wskaźnik na unikalną instancję silnika (Wzorzec Singleton).

    /**
     * @brief Konstruktor domyślny, inicjalizuje domyślne flagi graficzne i zmienne okna.
     */
    Engine();

    /**
     * @brief Inicjalizuje podsystemy biblioteki FreeGLUT, tworzy okno i ładuje zasoby sceny.
     * @param argc Wskaźnik do liczby argumentów systemowych.
     * @param argv Tablica argumentów systemowych.
     */
    void init(int* argc, char** argv);

    /**
     * @brief Uruchamia główną pętlę zdarzeń FreeGLUT.
     */
    void start();

    /**
     * @brief Zwalnia dynamicznie alokowaną pamięć obiektów oraz kamery przed wyjściem.
     */
    void deinit();

    /**
     * @brief Główna metoda rysująca klatkę sceny 3D oraz menu nakładki 2D.
     */
    void render();

    /**
     * @brief Obsługuje zmianę rozmiaru okna oraz przelicza aspekty rzutowania.
     * @param w Nowa szerokość okna.
     * @param h Nowa wysokość okna.
     */
    void reshape(int w, int h);

    /**
     * @brief Cykliczny tiktak zegara wywoływany przez glutTimerFunc, aktualizujący fizykę.
     * @param value Wartość przekazywana przez system zegara.
     */
    void update(int value);

    /**
     * @brief Przetwarza zdarzenie wciśnięcia klawisza.
     * @param key Kod ASCII znaku.
     * @param x Pozycja kursora X.
     * @param y Pozycja kursora Y.
     */
    void handleKeyDown(unsigned char key, int x, int y);

    /**
     * @brief Przetwarza zdarzenie zwolnienia klawisza.
     * @param key Kod ASCII znaku.
     * @param x Pozycja kursora X.
     * @param y Pozycja kursora Y.
     */
    void handleKeyUp(unsigned char key, int x, int y);

    /**
     * @brief Przetwarza swobodny ruch myszy bez wciśniętych przycisków w celu obracania kamery.
     * @param x Pozycja kursora X.
     * @param y Pozycja kursora Y.
     */
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