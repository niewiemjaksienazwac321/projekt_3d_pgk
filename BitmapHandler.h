#ifndef BITMAPHANDLER_H
#define BITMAPHANDLER_H

#include <GL/freeglut.h>
#include <string>

class BitmapHandler {
public:
    static GLuint loadTexture(const std::string& filename);
};

#endif