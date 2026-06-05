#include "Engine.h"

int main(int argc, char** argv) {
    Engine game;
    Engine::instance = &game;

    game.init(&argc, argv);
    game.start();

    return 0;
}