#ifndef CAMERA_H
#define CAMERA_H

#include "Vector.h"
#include "Matrix.h"

class Camera {
public:
    Vector Position;
    Vector Target;
    float FOV;
    float Aspect;

    Camera(Vector pos, Vector target, float fov, float aspect) 
        : Position(pos), Target(target), FOV(fov), Aspect(aspect) {}

    Matrix4 GetViewMatrix() const {
        return Matrix4::lookAt(Position, Target, Vector(0.0f, 1.0f, 0.0f));
    }

    Matrix4 GetProjectionMatrix() const {
        float fovRad = FOV * (3.14159f / 180.0f);
        return Matrix4::Perspective(fovRad, Aspect, 0.1f, 100.0f);
    }
};

#endif //CAMERA_H