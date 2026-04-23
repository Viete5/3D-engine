#include "../include/scene/light.hpp"

namespace engine::scene {

Light::Light(const engine::math::Vector& position, const engine::math::Vector& color)
    : position(position),
      color(color) {}


engine::math::Matrix4 Light::get_light_space_matrix() const {
    engine::math::Matrix4 ortho = engine::math::Matrix4::get_ortho(
        projection.ortho_left,
        projection.ortho_right,
        projection.ortho_bottom,
        projection.ortho_top,
        projection.near_plane,
        projection.far_plane
    );
    engine::math::Matrix4 light_view = engine::math::Matrix4::get_look_at(position, target, up);
    return ortho * light_view;
}

engine::math::Vector Light::get_position() const {
    return position;
}

engine::math::Vector Light::get_color() const {
    return color;
}

engine::math::Vector Light::get_target() const {
    return target;
}

engine::math::Vector Light::get_up() const {
    return up;
}

LightProjection Light::get_projection() const {
    return projection;
}

} // namespace engine::scene
