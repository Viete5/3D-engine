#ifndef CAMERA_H
#define CAMERA_H

#include "Vector.h"
#include "Matrix.h"
#include <string>

class Camera {
public:
    Vector Position;
    Vector Front;
    Vector Up;
    float FOV;
    float Aspect;
    const float Sensitivity;
    float Yaw;
    float Pitch;
    float Speed;

    Camera(Vector pos, Vector front, Vector up, float fov, float aspect);

    Matrix4 GetViewMatrix() const;

    Matrix4 GetProjectionMatrix() const;

    // Метод для обновления соотношения сторон
    void UpdateAspectRatio(float newAspectRatio);

    void UpdateAngle(float Xoffset, float Yoffset);
    void UpdateFront();

    void ProcessKeyboard(std::string direction, float deltaTime);
};

#endif //CAMERA_H