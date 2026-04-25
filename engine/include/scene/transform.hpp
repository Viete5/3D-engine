#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "../math/matrix.hpp"
#include "../math/vector.hpp"

namespace engine::scene {

class Transform {
public:
    Transform();
    Transform(
        const engine::math::Vector& position,
        const engine::math::Vector& rotation,
        const engine::math::Vector& scale
    );

    engine::math::Matrix4 get_model_matrix() const;

    engine::math::Vector get_position() const;
    engine::math::Vector get_rotation() const;
    engine::math::Vector get_scale() const;

    void set_position(const engine::math::Vector& new_position);
    void set_rotation(const engine::math::Vector& new_rotation);
    void set_scale(const engine::math::Vector& new_scale);

    void translate(const engine::math::Vector& offset);
    void rotate(const engine::math::Vector& angle_offset);
    void scale_by(const engine::math::Vector& scale_factor);

private:
    engine::math::Vector position = engine::math::Vector(0.0f, 0.0f, 0.0f);
    engine::math::Vector rotation = engine::math::Vector(0.0f, 0.0f, 0.0f);
    engine::math::Vector scale = engine::math::Vector(1.0f, 1.0f, 1.0f);

};

} // namespace engine::scene

#endif // TRANSFORM_HPP
