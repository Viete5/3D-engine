#include "../headers/Camera.h"


Camera::Camera(Vector pos, Vector front, Vector up, float fov, float aspect) 
    : Position(pos),
      Front(front),
      FOV(fov),
      Aspect(aspect),
      Up(up),
      Sensitivity(0.001f),
      Yaw(-1.57f),
      Pitch(0.0f),
      Speed(5.0f)
      {}


Matrix4 Camera::GetViewMatrix() const {
    return Matrix4::lookAt(Position, Front + Position, Up);
}

Matrix4 Camera::GetProjectionMatrix() const {
    float fovRad = FOV * (3.14159f / 180.0f);
    return Matrix4::Perspective(fovRad, Aspect, 0.1f, 100.0f);
}

void Camera::UpdateAspectRatio(float newAspectRatio) {
    Aspect = newAspectRatio;
}

void Camera::UpdateAngle(float Xoffset, float Yoffset) {
    Xoffset *= Sensitivity;
    Yoffset *= Sensitivity;

    Yaw += Xoffset;
    Pitch += Yoffset;

    
    if(Pitch > 1.5f) Pitch = 1.5f;
    if(Pitch <-1.5f) Pitch =-1.5f;

    UpdateFront();
}

void Camera::UpdateFront() {

    float x = std::cos(Yaw) * std::cos(Pitch);
    float y = std::sin(Pitch);
    float z = std::sin(Yaw) * std::cos(Pitch);
    Vector newCamFront(x,y,z);
    newCamFront.normalize();

    Front = newCamFront;
}

void Camera::ProcessKeyboard(std::string direction, float deltaTime) {
    float velocity = Speed * deltaTime;
    if (direction == "FORWARD")  Position = Position + Front * velocity;
    if (direction == "BACKWARD") Position = Position - Front * velocity;
    if (direction == "LEFT")     Position = Position - Front.crossprod(Up) * velocity;
    if (direction == "RIGHT")    Position = Position + Front.crossprod(Up) * velocity;
    if (direction == "UP")       Position.setY(Position.getY() + velocity);
    if (direction == "DOWN")     Position.setY(Position.getY() - velocity);
}