/**
 * @file GeometryProvider.h
 * @brief Polimorficzna hierarchia struktur brył geometrycznych renderowanych za pomocą tablic wierzchołków.
 */

#ifndef GEOMETRY_PROVIDER_H
#define GEOMETRY_PROVIDER_H

#include <GL/freeglut.h>
#include <vector>

/**
 * @class Object3D
 * @brief Abstrakcyjna klasa bazowa reprezentująca obiekt trójwymiarowy na scenie.
 */
class Object3D {
public:
    float posX, posY, posZ;       ///< Współrzędne położenia obiektu w świecie.
    float rotX, rotY, rotZ;       ///< Kąty obrotu obiektu wokół własnych osi lokalnych.
    float scaleX, scaleY, scaleZ; ///< Mnożniki skalowania obiektu w trzech osiach.
    GLuint textureId;             ///< Domyślny identyfikator tekstury obiektu.

    /**
     * @brief Inicjalizuje transformacje przestrzenne obiektu.
     */
    Object3D(float x = 0.0f, float y = 0.0f, float z = 0.0f);
    virtual ~Object3D() {}

    /**
     * @brief Aplikuje transformacje translacji, rotacji oraz skalowania do macierzy stanów.
     */
    void applyTransformations() const;

    /**
     * @brief Czysto wirtualna funkcja renderująca model geometryczny bryły.
     * @param isShadow Określa, czy bryła ma być rysowana jako czarny kształt cienia.
     */
    virtual void draw(bool isShadow = false) = 0;
};

class TriangleObject : public Object3D {
public:
    TriangleObject(float x = 0.0f, float y = 0.0f, float z = 0.0f) : Object3D(x, y, z) {}
    void draw(bool isShadow = false) override;
};

class CubeObject : public Object3D {
public:
    GLuint textureTopId; ///< Opcjonalna dedykowana tekstura dla wierzchołka sześcianu.
    CubeObject(float x = 0.0f, float y = 0.0f, float z = 0.0f) : Object3D(x, y, z), textureTopId(0) {}
    void draw(bool isShadow = false) override;
};

class PyramidObject : public Object3D {
public:
    PyramidObject(float x = 0.0f, float y = 0.0f, float z = 0.0f) : Object3D(x, y, z) {}
    void draw(bool isShadow = false) override;
};

class FloorObject : public Object3D {
public:
    FloorObject(float x = 0.0f, float y = 0.0f, float z = 0.0f) : Object3D(x, y, z) {}
    void draw(bool isShadow = false) override;
};

class SphereObject : public Object3D {
private:
    std::vector<float> vertices; ///< Tablica współrzędnych wierzchołków sfery.
    std::vector<float> normals;  ///< Tablica wektorów normalnych dla poprawnego oświetlenia.
public:
    SphereObject(float x = 0.0f, float y = 0.0f, float z = 0.0f);
    void draw(bool isShadow = false) override;
};

class ConeObject : public Object3D {
private:
    std::vector<float> vertices; ///< Tablica wierzchołków stożka.
    std::vector<float> normals;  ///< Tablica normalnych powierzchni stożka.
public:
    ConeObject(float x = 0.0f, float y = 0.0f, float z = 0.0f);
    void draw(bool isShadow = false) override;
};

class GeometryProvider {
public:
    static std::vector<Object3D*> initScene();
};

#endif