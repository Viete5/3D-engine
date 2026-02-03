#ifndef LIGHT_H
#define LIGHT_H

#include "Matrix.h"
#include "Vector.h"

class Light {
public:
    Vector Position;
    Vector Color;
    Light(Vector pos, Vector col) : Position(pos), Color(col) {}
    Matrix4 GetLightSpaceMatrix() const {
        Matrix4 ortho = Matrix4::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
        Matrix4 lightView = Matrix4::lookAt(Position, Vector(0,0,0), Vector(0,1,0));
        return ortho * lightView;
    }
};



#endif //LIGHT_H