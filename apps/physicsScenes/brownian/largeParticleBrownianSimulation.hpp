#ifndef LARGE_PARTICLE_BROWNIAN_SIMULATION_HPP
#define LARGE_PARTICLE_BROWNIAN_SIMULATION_HPP

#include "brownianSimulation.hpp"

#include "../../../engine/include/math/vector.hpp"

#include <cstddef>

struct LargeParticleBrownianSimulationSettings {
    int particle_count = 500;
    int max_particle_count = 1200;
    int density_step = 50;
    engine::math::Vector bounds = engine::math::Vector(3.0f, 3.0f, 3.0f);
    float small_radius = 0.06f;
    float large_radius = 0.25f;
    float small_mass = 1.0f;
    float large_mass = 3.0f;
    float base_speed = 2.0f;

    float attractor_radius = 0.45f;
    float attractor_mass = 3.0f;
    float attractor_speed = 1.4f;
    float attractor_gravity_coeff = 1.5f;
    float attractor_softening = 0.25f;

    float initial_temperature = 1.15f;
    float restitution = 0.98f;
    float large_particle_damping = 1.0f;
};

class LargeParticleBrownianSimulation : public ParticleSimulationBase {
public:
    LargeParticleBrownianSimulation();
    LargeParticleBrownianSimulation(const LargeParticleBrownianSimulationSettings& settings);

    void update(float delta_time) override;
    void reset() override;

    void set_particle_count(int new_count);
    void change_particle_count(int delta);

    void toggle_attractor();
    void create_attractor();
    void remove_attractor();

    const BrownianParticle& get_attractor() const;
    bool get_attractor_active() const;
    std::size_t get_attractor_revision() const;

    const BrownianParticle& get_large_particle() const;
    const LargeParticleBrownianSimulationSettings& get_settings() const;
    int get_particle_count() const;
    std::size_t get_particle_revision() const;

private:
    LargeParticleBrownianSimulationSettings settings;
    BrownianParticle large_particle;
    std::size_t particle_revision = 0;

    BrownianParticle attractor;
    bool attractor_active = false;
    std::size_t attractor_revision = 0;

    void apply_attractor_force(float delta_time);
    BrownianParticle create_small_particle();
    void add_particle();
    void remove_particles(int remove_count);
    void resolve_large_particle_collisions();
};

#endif // LARGE_PARTICLE_BROWNIAN_SIMULATION_HPP
