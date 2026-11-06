/**
 * @file GeometryProvider.h
 * @brief Polimorficzna hierarchia struktur brył geometrycznych renderowanych za pomocą tablic wierzchołków.
 */

#ifndef GEOMETRY_PROVIDER_H
#define GEOMETRY_PROVIDER_H

#include <GL/freeglut.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
class HeadObject : public SphereObject {
public:
    HeadObject(float x = 0.0f, float y = 0.0f, float z = 0.0f) : SphereObject(x, y, z) {}
    void draw(bool isShadow = false) override;
};
class MovingPlatformObject : public CubeObject {
public:
    glm::vec3 startPos;
    glm::vec3 endPos;
    float speed;
    float t;          // Parametr interpolacji (0.0 do 1.0)
    bool forward;     // Kierunek ruchu

    MovingPlatformObject(glm::vec3 start, glm::vec3 end, float spd = 0.01f, float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : CubeObject(x, y, z), startPos(start), endPos(end), speed(spd), t(0.0f), forward(true) {
        posX = start.x;
        posY = start.y;
        posZ = start.z;
    }

    /**
     * @brief Aktualizuje pozycję platformy w pętli symulacji.
     * @return Wektor przemieszczenia (delta) w tej klatce, potrzebny do przesunięcia gracza.
     */
    glm::vec3 updatePosition() {
        glm::vec3 oldPos(posX, posY, posZ);
        if (forward) {
            t += speed;
            if (t >= 1.0f) { t = 1.0f; forward = false; }
        } else {
            t -= speed;
            if (t <= 0.0f) { t = 0.0f; forward = true; }
        }
        glm::vec3 current = glm::mix(startPos, endPos, t);
        posX = current.x;
        posY = current.y;
        posZ = current.z;
        return glm::vec3(posX - oldPos.x, posY - oldPos.y, posZ - oldPos.z);
    }
};

class SlopePlatformObject : public Object3D {
public:
    SlopePlatformObject(float x = 0.0f, float y = 0.0f, float z = 0.0f) : Object3D(x, y, z) {}
    void draw(bool isShadow = false) override;

    /**
     * @brief Zwraca dokładną wysokość (Y) rampy w lokalnych współrzędnych (x, z).
     */
    float getHeightAtLocal(float lx, float lz) const {
        if (lx < -0.5f || lx > 0.5f || lz < -0.5f || lz > 0.5f) return -999.0f;
        float normalizedZ = (lz + 0.5f);
        return normalizedZ * scaleY;
    }
};

class IcePlatformObject : public CubeObject {
public:
    GLuint textureId = 0;
    IcePlatformObject(float x = 0.0f, float y = 0.0f, float z = 0.0f) : CubeObject(x, y, z) {}
    void draw(bool isShadow = false) override;
};

class IceSlopeObject : public SlopePlatformObject {
public:
    GLuint textureId = 0;
    IceSlopeObject(float x = 0.0f, float y = 0.0f, float z = 0.0f) : SlopePlatformObject(x, y, z) {}
    void draw(bool isShadow = false) override;
};
class LavaObject : public CubeObject {
public:
    GLuint textureId = 0;
    LavaObject(float x = 0.0f, float y = 0.0f, float z = 0.0f) : CubeObject(x, y, z) {}
    void draw(bool isShadow = false) override;
};
class GeometryProvider {
public:
    static std::vector<Object3D*> initScene();
};

#endif