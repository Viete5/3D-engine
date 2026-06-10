#ifndef BROWNIAN_SIMULATION_HPP
#define BROWNIAN_SIMULATION_HPP

#include "brownianParticle.hpp"
#include "dataStructures/dynamicArray.hpp"

#include "../../../engine/include/math/vector.hpp"

#include <random>

// электрическое поле. Аттрактор. Управление большой частицы

class ParticleSimulationBase {
public:
    virtual ~ParticleSimulationBase() = default;

    virtual void update(float delta_time) = 0;
    virtual void reset() = 0;

    void set_temperature(float new_temperature);
    void set_paused(bool value);
    void toggle_pause();

    const brownian::data_structures::DynamicArray<BrownianParticle>& get_particles() const;
    float get_temperature() const;
    bool get_paused() const;

protected:
    static constexpr float minimum_distance = 1e-5f;

    ParticleSimulationBase(
        const engine::math::Vector& bounds,
        float temperature,
        float restitution
    );

    float random_float(float min_value, float max_value);
    engine::math::Vector random_direction();
    engine::math::Vector random_position_inside_bounds(float radius);

    static float clamp_temperature(float value);

    void move_particle(BrownianParticle& particle, float delta_time) const;
    void resolve_wall_collision(BrownianParticle& particle) const;
    void resolve_wall_collision(BrownianParticle& particle, float wall_restitution) const;
    void resolve_particle_collision(BrownianParticle& first, BrownianParticle& second) const;
    void resolve_particle_collisions();

    brownian::data_structures::DynamicArray<BrownianParticle> particles;
    std::mt19937 generator;
    engine::math::Vector bounds;
    float temperature = 1.0f;
    float restitution = 0.98f;
    bool paused = false;
};

#endif // BROWNIAN_SIMULATION_HPP
