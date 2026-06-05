#ifndef GEOMETRY_PROVIDER_H
#define GEOMETRY_PROVIDER_H

#include <GL/freeglut.h>
#include <vector>

class Object3D {
public:
    float posX, posY, posZ;
    float rotX, rotY, rotZ;
    float scaleX, scaleY, scaleZ;
    GLuint textureId;

    Object3D(float x = 0.0f, float y = 0.0f, float z = 0.0f);
    virtual ~Object3D() {}

    void applyTransformations() const;
    virtual void draw(bool isShadow = false) = 0;
};

class TriangleObject : public Object3D {
public:
    TriangleObject(float x = 0.0f, float y = 0.0f, float z = 0.0f) : Object3D(x, y, z) {}
    void draw(bool isShadow = false) override;
};

class CubeObject : public Object3D {
public:
    GLuint textureTopId;

    CubeObject(float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : Object3D(x, y, z), textureTopId(0) {}

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

// NOWE: Sfery i Stożki oparte w całości na naszym systemie tablic
class SphereObject : public Object3D {
private:
    std::vector<float> vertices;
    std::vector<float> normals;
public:
    SphereObject(float x = 0.0f, float y = 0.0f, float z = 0.0f);
    void draw(bool isShadow = false) override;
};

class ConeObject : public Object3D {
private:
    std::vector<float> vertices;
    std::vector<float> normals;
public:
    ConeObject(float x = 0.0f, float y = 0.0f, float z = 0.0f);
    void draw(bool isShadow = false) override;
};

class GeometryProvider {
public:
    static std::vector<Object3D*> initScene();
};

#endif