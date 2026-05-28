#ifndef BROWNIAN_PARTICLE_HPP
#define BROWNIAN_PARTICLE_HPP

#include "../../../engine/include/math/vector.hpp"

struct BrownianParticle {
    engine::math::Vector position;
    engine::math::Vector velocity;
    engine::math::Vector color;
    float radius;
    float mass;
};

#endif // BROWNIAN_PARTICLE_HPP
