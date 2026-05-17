#include "../include/scene/camera.hpp"
#include "../include/math/matrix.hpp"

namespace engine::scene {

Camera::Camera(const engine::math::Vector& pos, const engine::math::Vector& front, const engine::math::Vector& up, float fov, float aspect) 
    : position(pos),
      front(front),
      fov(fov),
      aspect(aspect),
      up(up),
      sensitivity(0.001f),
      yaw(-1.57f),
      pitch(0.0f),
      speed(5.0f)
      {}


engine::math::Matrix4 Camera::get_view_matrix() const {
    return engine::math::Matrix4::get_look_at(position, front + position, up);
}

engine::math::Matrix4 Camera::get_projection_matrix() const {
    float fov_rad = fov * (3.14159265358979f / 180.0f);
    return engine::math::Matrix4::get_perspective(fov_rad, aspect, near, far);
}

void Camera::update_aspect_ratio(float new_aspect_ratio) {
    aspect = new_aspect_ratio;
}

void Camera::set_fov(float new_fov) {
    fov = new_fov;
}

void Camera::update_angle(float x_offset, float y_offset) {
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    yaw += x_offset;
    pitch += y_offset;

    
    if(pitch > pitch_limit) pitch = pitch_limit;
    if(pitch <-pitch_limit) pitch = -pitch_limit;

    update_front();
}

void Camera::update_front() {

    float x = std::cos(yaw) * std::cos(pitch);
    float y = std::sin(pitch);
    float z = std::sin(yaw) * std::cos(pitch);
    engine::math::Vector new_cam_front(x,y,z);

    front = new_cam_front.normalize();
}

void Camera::process_keyboard(CameraMovement direction, float delta_time) {
    float velocity = speed * delta_time;
    switch (direction) {
        case CameraMovement::Forward:
            position = position + front * velocity;
            break;
        case CameraMovement::Backward:
            position = position - front * velocity;
            break;
        case CameraMovement::Left:
            position = position - front.crossprod(up).normalize() * velocity;
            break;
        case CameraMovement::Right:
            position = position + front.crossprod(up).normalize() * velocity;
            break;
        case CameraMovement::Up:
            position.set_y(position.get_y() + velocity);
            break;
        case CameraMovement::Down:
            position.set_y(position.get_y() - velocity);
            break;
    }
}

} // namespace engine::scene
