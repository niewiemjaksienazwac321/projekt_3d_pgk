/**
* @file main.cpp
 * @brief Główny plik rozruchowy aplikacji gry trójwymiarowej.
 */

#include "Engine.h"

/**
 * @brief Główny punkt wejścia do aplikacji. Inicjalizuje Singleton silnika i uruchamia pętlę renderowania.
 * @param argc Liczba argumentów linii komend.
 * @param argv Tablica ciągów znaków argumentów linii komend.
 * @return Kod zakończenia programu (0 oznacza sukces).
 */
int main(int argc, char** argv) {
    Engine game;
    Engine::instance = &game;

    game.init(&argc, argv);
    game.start();

    return 0;
}