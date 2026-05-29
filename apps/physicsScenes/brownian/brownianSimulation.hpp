#ifndef BROWNIAN_SIMULATION_HPP
#define BROWNIAN_SIMULATION_HPP

#include <vector>
#include "brownianParticle.hpp"

// электрическое поле. Аттрактор. Управление большой частицы

struct BrownianSimulationSettings {
    int particle_count = 500;
    engine::math::Vector bounds = engine::math::Vector(3.0f, 3.0f, 3.0f);
    float small_radius = 0.06f;
    float large_radius = 0.25f;
    float small_mass = 1.0f;
    float large_mass = 3.0f;
    float base_speed = 2.0f;
    float temperature = 1.15f;
    float restitution = 0.98f;
    float large_particle_damping = 1.0f;
};

class BrownianSimulation {
public:
    BrownianSimulation();
    BrownianSimulation(const BrownianSimulationSettings& settings);

    void update(float delta_time);
    void reset();

    void set_temperature(float new_temperature);
    void set_paused(bool value);
    void toggle_pause();

    const std::vector<BrownianParticle>& get_particles() const;
    const BrownianParticle& get_large_particle() const;
    const BrownianSimulationSettings& get_settings() const;
    float get_temperature() const;
    bool get_paused() const;

private:
    BrownianSimulationSettings settings;
    std::vector<BrownianParticle> particles;
    BrownianParticle large_particle;
    bool paused = false;

    void move_particle(BrownianParticle& particle, float delta_time) const;
    void resolve_wall_collision(BrownianParticle& particle) const;
    void resolve_particle_collision(BrownianParticle& first, BrownianParticle& second) const;
    void resolve_small_particle_collisions();
    void resolve_large_particle_collisions();
};


#endif // BROWNIAN_SIMULATION_HPP
