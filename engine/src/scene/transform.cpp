#include "../include/scene/transform.hpp"

namespace engine::scene {

Transform::Transform() = default;

Transform::Transform(
    const engine::math::Vector& position,
    const engine::math::Vector& rotation,
    const engine::math::Vector& scale
)
    : position(position),
      rotation(rotation),
      scale(scale) {}

engine::math::Matrix4 Transform::get_model_matrix() const {
    engine::math::Matrix4 translate = engine::math::Matrix4::get_translate(
        position.get_x(),
        position.get_y(),
        position.get_z()
    );

    engine::math::Matrix4 rotate_x = engine::math::Matrix4::get_rotate_x(rotation.get_x());
    engine::math::Matrix4 rotate_y = engine::math::Matrix4::get_rotate_y(rotation.get_y());
    engine::math::Matrix4 rotate_z = engine::math::Matrix4::get_rotate_z(rotation.get_z());

    engine::math::Matrix4 scale = engine::math::Matrix4::get_scale(
        this->scale.get_x(),
        this->scale.get_y(),
        this->scale.get_z()
    );

    return translate * rotate_z * rotate_y * rotate_x * scale;
}

engine::math::Vector Transform::get_position() const {
    return position;
}

engine::math::Vector Transform::get_rotation() const {
    return rotation;
}

engine::math::Vector Transform::get_scale() const {
    return scale;
}

void Transform::set_position(const engine::math::Vector& new_position) {
    position = new_position;
}

void Transform::set_rotation(const engine::math::Vector& new_rotation) {
    rotation = new_rotation;
}

void Transform::set_scale(const engine::math::Vector& new_scale) {
    scale = new_scale;
}

void Transform::translate(const engine::math::Vector& offset) {
    position = position + offset;
}

void Transform::rotate(const engine::math::Vector& angle_offset) {
    rotation = rotation + angle_offset;
}

void Transform::scale_by(const engine::math::Vector& scale_factor) {
    scale = engine::math::Vector(
        scale.get_x() * scale_factor.get_x(),
        scale.get_y() * scale_factor.get_y(),
        scale.get_z() * scale_factor.get_z()
    );
}

} // namespace engine::scene
