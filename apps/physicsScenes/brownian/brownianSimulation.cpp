#include "brownianSimulation.hpp"

#include <algorithm>
#include <cmath>
#include <random>

namespace {

constexpr float min_distance = 1e-5f;

float random_float(std::mt19937& generator, float min_value, float max_value) {
    std::uniform_real_distribution<float> distribution(min_value, max_value);
    return distribution(generator);
}

engine::math::Vector random_direction(std::mt19937& generator) {
    engine::math::Vector direction(
        random_float(generator, -1.0f, 1.0f),
        random_float(generator, -1.0f, 1.0f),
        random_float(generator, -1.0f, 1.0f)
    );

    if (direction.dist() <= min_distance) {
        return engine::math::Vector(1.0f, 0.0f, 0.0f);
    }

    return direction.normalize();
}

engine::math::Vector random_position_inside_bounds(
    std::mt19937& generator,
    const engine::math::Vector& bounds,
    float radius
) {
    return engine::math::Vector(
        random_float(generator, -bounds.get_x() + radius, bounds.get_x() - radius),
        random_float(generator, -bounds.get_y() + radius, bounds.get_y() - radius),
        random_float(generator, -bounds.get_z() + radius, bounds.get_z() - radius)
    );
}

float clamp_temperature(float value) {
    return std::max(0.05f, value);
}

void clamp_axis(float& position, float& velocity, float limit) {
    if (position > limit) {
        position = limit;
        velocity = -std::abs(velocity);
    } else if (position < -limit) {
        position = -limit;
        velocity = std::abs(velocity);
    }
}

} // namespace

BrownianSimulation::BrownianSimulation()
    : BrownianSimulation(BrownianSimulationSettings{}) {
}

BrownianSimulation::BrownianSimulation(const BrownianSimulationSettings& settings)
    : settings(settings) {
    reset();
}

void BrownianSimulation::reset() {
    settings.temperature = clamp_temperature(settings.temperature);
    paused = false;
    particles.clear();
    particles.reserve(static_cast<std::size_t>(std::max(0, settings.particle_count)));

    std::random_device random_device;
    std::mt19937 generator(random_device());

    large_particle.position = engine::math::Vector(0.0f, 0.0f, 0.0f);
    large_particle.velocity = engine::math::Vector(0.0f, 0.0f, 0.0f);
    large_particle.color = engine::math::Vector(1.0f, 0.75f, 0.15f);
    large_particle.radius = settings.large_radius;
    large_particle.mass = settings.large_mass;

    for (int index = 0; index < settings.particle_count; ++index) {
        BrownianParticle particle;
        particle.radius = settings.small_radius;
        particle.mass = settings.small_mass;
        particle.color = engine::math::Vector(0.2f, 0.55f, 1.0f);
        particle.velocity = random_direction(generator) * (settings.base_speed * settings.temperature);

        int attempts = 0;
        do {
            particle.position = random_position_inside_bounds(generator, settings.bounds, particle.radius);
            ++attempts;
        } while (
            particle.position.dist_between_dots(large_particle.position) <
                particle.radius + large_particle.radius &&
            attempts < 32
        );

        particles.push_back(particle);
    }
}

void BrownianSimulation::update(float delta_time) {
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

    resolve_small_particle_collisions();
    resolve_large_particle_collisions();
}

void BrownianSimulation::set_temperature(float new_temperature) {
    new_temperature = clamp_temperature(new_temperature);
    const float ratio = new_temperature / settings.temperature;
    settings.temperature = new_temperature;

    for (BrownianParticle& particle : particles) {
        particle.velocity = particle.velocity * ratio;
    }
}

void BrownianSimulation::set_paused(bool value) {
    paused = value;
}

void BrownianSimulation::toggle_pause() {
    paused = !paused;
}

const std::vector<BrownianParticle>& BrownianSimulation::get_particles() const {
    return particles;
}

const BrownianParticle& BrownianSimulation::get_large_particle() const {
    return large_particle;
}

const BrownianSimulationSettings& BrownianSimulation::get_settings() const {
    return settings;
}

float BrownianSimulation::get_temperature() const {
    return settings.temperature;
}

bool BrownianSimulation::get_paused() const {
    return paused;
}

void BrownianSimulation::move_particle(BrownianParticle& particle, float delta_time) const {
    particle.position = particle.position + particle.velocity * delta_time;
}

void BrownianSimulation::resolve_wall_collision(BrownianParticle& particle) const {
    float x = particle.position.get_x();
    float y = particle.position.get_y();
    float z = particle.position.get_z();

    float velocity_x = particle.velocity.get_x();
    float velocity_y = particle.velocity.get_y();
    float velocity_z = particle.velocity.get_z();

    clamp_axis(x, velocity_x, settings.bounds.get_x() - particle.radius);
    clamp_axis(y, velocity_y, settings.bounds.get_y() - particle.radius);
    clamp_axis(z, velocity_z, settings.bounds.get_z() - particle.radius);

    particle.position = engine::math::Vector(x, y, z);
    particle.velocity = engine::math::Vector(velocity_x, velocity_y, velocity_z);
}

void BrownianSimulation::resolve_particle_collision(BrownianParticle& first, BrownianParticle& second) const {
    engine::math::Vector delta = second.position - first.position;
    float distance = delta.dist();
    const float collision_distance = first.radius + second.radius;

    if (distance >= collision_distance) {
        return;
    }

    engine::math::Vector normal(1.0f, 0.0f, 0.0f);
    if (distance > min_distance) {
        normal = delta * (1.0f / distance);
    } else {
        distance = min_distance;
    }

    const float inverse_first_mass = 1.0f / first.mass;
    const float inverse_second_mass = 1.0f / second.mass;
    const float inverse_mass_sum = inverse_first_mass + inverse_second_mass;
    if (inverse_mass_sum <= min_distance) {
        return;
    }

    const float penetration = collision_distance - distance;
    first.position = first.position - normal * (penetration * (inverse_first_mass / inverse_mass_sum));
    second.position = second.position + normal * (penetration * (inverse_second_mass / inverse_mass_sum));

    const engine::math::Vector relative_velocity = second.velocity - first.velocity;
    const float velocity_along_normal = relative_velocity * normal;
    if (velocity_along_normal > 0.0f) {
        return;
    }

    const float impulse_scalar = -(1.0f + settings.restitution) * velocity_along_normal / inverse_mass_sum;
    const engine::math::Vector impulse = normal * impulse_scalar;

    first.velocity = first.velocity - impulse * inverse_first_mass;
    second.velocity = second.velocity + impulse * inverse_second_mass;
}

void BrownianSimulation::resolve_small_particle_collisions() {
    for (std::size_t first_index = 0; first_index < particles.size(); ++first_index) {
        for (std::size_t second_index = first_index + 1; second_index < particles.size(); ++second_index) {
            resolve_particle_collision(particles[first_index], particles[second_index]);
        }
    }
}

void BrownianSimulation::resolve_large_particle_collisions() {
    for (BrownianParticle& particle : particles) {
        resolve_particle_collision(particle, large_particle);
    }
}
