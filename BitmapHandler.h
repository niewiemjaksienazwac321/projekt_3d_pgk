/**
* @file BitmapHandler.h
 * @brief Statyczny interfejs pomocniczy ładowania plików tekstur rastrowych.
 */

#ifndef BITMAPHANDLER_H
#define BITMAPHANDLER_H

#include <GL/freeglut.h>
#include <string>

/**
 * @class BitmapHandler
 * @brief Klasa kapsułkująca proces ładowania danych graficznych do struktur tekstur OpenGL.
 */
class BitmapHandler {
public:
    /**
     * @brief Generuje teksturę w pamięci GPU na podstawie podanej ścieżki pliku źródłowego.
     * @param filename Nazwa pliku graficznego wraz z rozszerzeniem.
     * @return Identyfikator wygenerowanego obiektu tekstury (GLuint).
     */
    static GLuint loadTexture(const std::string& filename);
};

#endif