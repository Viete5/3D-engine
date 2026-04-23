#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../math/vector.hpp"

namespace engine::math {
class Matrix4;
}

namespace engine::scene {

enum class CameraMovement {
    Forward,
    Backward,
    Left,
    Right,
    Up,
    Down
};


class Camera {
public:
    Camera(const engine::math::Vector& pos, const engine::math::Vector& front, const engine::math::Vector& up, float fov, float aspect);

    engine::math::Matrix4 get_view_matrix() const;

    engine::math::Matrix4 get_projection_matrix() const;

    // Метод для обновления соотношения сторон
    void update_aspect_ratio(float new_aspect_ratio);

    void update_angle(float x_offset, float y_offset);
    void update_front();

    void process_keyboard(CameraMovement direction, float delta_time);

    engine::math::Vector get_position() const {return position;}
    engine::math::Vector get_front() const {return front;}
    engine::math::Vector get_up() const {return up;}
    float get_fov() const {return fov;}
    float get_aspect() const {return aspect;}
    float get_sensitivity() const {return sensitivity;}
    float get_yaw() const {return yaw;}
    float get_pitch() const {return pitch;}
    float get_speed() const {return speed;}

private:
    engine::math::Vector position;
    engine::math::Vector front;
    engine::math::Vector up;
    float fov;
    float aspect;
    const float sensitivity;
    float yaw;
    float pitch;
    float speed;
    float near = 0.1f;
    float far = 100.0f;
    float pitch_limit = 1.5f;
};

} // engine::scene

#endif // CAMERA_HPP