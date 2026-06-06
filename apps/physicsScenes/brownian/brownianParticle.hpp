#ifndef BROWNIAN_PARTICLE_HPP
#define BROWNIAN_PARTICLE_HPP

#include "../../../engine/include/math/vector.hpp"

enum class BrownianGasType {
    GasA,
    GasB
};

struct BrownianParticle {
    engine::math::Vector position;
    engine::math::Vector previous_position;
    engine::math::Vector velocity;
    engine::math::Vector color;
    float radius;
    float mass;
    BrownianGasType gas_type = BrownianGasType::GasA;
};

#endif // BROWNIAN_PARTICLE_HPP
