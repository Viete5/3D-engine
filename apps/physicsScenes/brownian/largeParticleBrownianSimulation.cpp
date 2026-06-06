#include "largeParticleBrownianSimulation.hpp"

#include <algorithm>

namespace {

constexpr int spawn_attempt_count = 32;

} // namespace

LargeParticleBrownianSimulation::LargeParticleBrownianSimulation()
    : LargeParticleBrownianSimulation(LargeParticleBrownianSimulationSettings{}) {
}

LargeParticleBrownianSimulation::LargeParticleBrownianSimulation(
    const LargeParticleBrownianSimulationSettings& settings
)
    : ParticleSimulationBase(settings.bounds, settings.initial_temperature, settings.restitution),
      settings(settings) {
    reset();
}

void LargeParticleBrownianSimulation::reset() {
    temperature = clamp_temperature(settings.initial_temperature);
    settings.particle_count = std::clamp(settings.particle_count, 0, settings.max_particle_count);

    particles.clear();
    particles.reserve(static_cast<std::size_t>(std::max(0, settings.particle_count)));

    large_particle.position = engine::math::Vector(0.0f, 0.0f, 0.0f);
    large_particle.previous_position = large_particle.position;
    large_particle.velocity = engine::math::Vector(0.0f, 0.0f, 0.0f);
    large_particle.color = engine::math::Vector(1.0f, 0.75f, 0.15f);
    large_particle.radius = settings.large_radius;
    large_particle.mass = settings.large_mass;

    for (int index = 0; index < settings.particle_count; ++index) {
        particles.push_back(create_small_particle());
    }

    ++particle_revision;
}

void LargeParticleBrownianSimulation::update(float delta_time) {
    if (paused || delta_time <= 0.0f) {
        return;
    }

    for (BrownianParticle& particle : particles) {
        move_particle(particle, delta_time);
        resolve_wall_collision(particle);
    }

    move_particle(large_particle, delta_time);
    large_particle.velocity = large_particle.velocity * settings.large_particle_damping;
    resolve_wall_collision(large_particle);

    resolve_particle_collisions();
    resolve_large_particle_collisions();
}

const BrownianParticle& LargeParticleBrownianSimulation::get_large_particle() const {
    return large_particle;
}

const LargeParticleBrownianSimulationSettings& LargeParticleBrownianSimulation::get_settings() const {
    return settings;
}

void LargeParticleBrownianSimulation::set_particle_count(int new_count) {
    new_count = std::clamp(new_count, 0, settings.max_particle_count);

    if (new_count == settings.particle_count) {
        return;
    }

    if (new_count > settings.particle_count) {
        particles.reserve(static_cast<std::size_t>(new_count));
        const int add_count = new_count - settings.particle_count;
        for (int index = 0; index < add_count; ++index) {
            add_particle();
        }
    } else {
        remove_particles(settings.particle_count - new_count);
    }

    settings.particle_count = new_count;
    ++particle_revision;
}

void LargeParticleBrownianSimulation::change_particle_count(int delta) {
    set_particle_count(settings.particle_count + delta);
}

int LargeParticleBrownianSimulation::get_particle_count() const {
    return settings.particle_count;
}

std::size_t LargeParticleBrownianSimulation::get_particle_revision() const {
    return particle_revision;
}

BrownianParticle LargeParticleBrownianSimulation::create_small_particle() {
    BrownianParticle particle;
    particle.radius = settings.small_radius;
    particle.mass = settings.small_mass;
    particle.color = engine::math::Vector(0.2f, 0.55f, 1.0f);
    particle.velocity = random_direction() * (settings.base_speed * temperature);

    int attempts = 0;
    do {
        particle.position = random_position_inside_bounds(particle.radius);
        particle.previous_position = particle.position;
        ++attempts;
    } while (
        particle.position.dist_between_dots(large_particle.position) <
            particle.radius + large_particle.radius &&
        attempts < spawn_attempt_count
    );

    return particle;
}

void LargeParticleBrownianSimulation::add_particle() {
    particles.push_back(create_small_particle());
}

void LargeParticleBrownianSimulation::remove_particles(int remove_count) {
    if (remove_count <= 0 || particles.empty()) {
        return;
    }

    const std::size_t current_count = particles.size();
    const std::size_t count_to_remove = std::min(
        static_cast<std::size_t>(remove_count),
        current_count
    );

    particles.resize(current_count - count_to_remove);
}

void LargeParticleBrownianSimulation::resolve_large_particle_collisions() {
    for (BrownianParticle& particle : particles) {
        resolve_particle_collision(particle, large_particle);
    }
}
