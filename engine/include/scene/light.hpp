#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "../math/matrix.hpp"
#include "../math/vector.hpp"

namespace engine::scene {

struct LightProjection {
    float ortho_left = -10.0f;
    float ortho_right = 10.0f;
    float ortho_bottom = -10.0f;
    float ortho_top = 10.0f;
    float near_plane = 0.1f;
    float far_plane = 100.0f;
};

class Light {
public:
    Light(const engine::math::Vector& position, const engine::math::Vector& color);
    engine::math::Matrix4 get_light_space_matrix() const;

    engine::math::Vector get_position() const;
    engine::math::Vector get_color() const;
    engine::math::Vector get_target() const;
    engine::math::Vector get_up() const;
    LightProjection get_projection() const;

private:
    engine::math::Vector position;
    engine::math::Vector color;
    engine::math::Vector target = engine::math::Vector(0.0f, 0.0f, 0.0f);
    engine::math::Vector up = engine::math::Vector(0.0f, 1.0f, 0.0f);
    LightProjection projection;
};


} // namespace engine::scene

#endif
