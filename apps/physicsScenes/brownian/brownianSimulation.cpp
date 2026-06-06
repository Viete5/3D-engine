#include "brownianSimulation.hpp"

#include <algorithm>
#include <cmath>
#include <random>

namespace {

void clamp_axis(float& position, float& velocity, float limit, float restitution) {
    if (position > limit) {
        position = limit;
        velocity = -std::abs(velocity) * restitution;
    } else if (position < -limit) {
        position = -limit;
        velocity = std::abs(velocity) * restitution;
    }
}

} // namespace

ParticleSimulationBase::ParticleSimulationBase(
    const engine::math::Vector& bounds,
    float temperature,
    float restitution
)
    : generator(std::random_device{}()),
      bounds(bounds),
      temperature(clamp_temperature(temperature)),
      restitution(restitution) {
}

void ParticleSimulationBase::set_temperature(float new_temperature) {
    new_temperature = clamp_temperature(new_temperature);
    const float ratio = new_temperature / temperature;
    temperature = new_temperature;

    for (BrownianParticle& particle : particles) {
        particle.velocity = particle.velocity * ratio;
    }
}

void ParticleSimulationBase::set_paused(bool value) {
    paused = value;
}

void ParticleSimulationBase::toggle_pause() {
    paused = !paused;
}

const std::vector<BrownianParticle>& ParticleSimulationBase::get_particles() const {
    return particles;
}

float ParticleSimulationBase::get_temperature() const {
    return temperature;
}

bool ParticleSimulationBase::get_paused() const {
    return paused;
}

float ParticleSimulationBase::random_float(float min_value, float max_value) {
    std::uniform_real_distribution<float> distribution(min_value, max_value);
    return distribution(generator);
}

engine::math::Vector ParticleSimulationBase::random_direction() {
    engine::math::Vector direction(
        random_float(-1.0f, 1.0f),
        random_float(-1.0f, 1.0f),
        random_float(-1.0f, 1.0f)
    );

    if (direction.dist() <= minimum_distance) {
        return engine::math::Vector(1.0f, 0.0f, 0.0f);
    }

    return direction.normalize();
}

engine::math::Vector ParticleSimulationBase::random_position_inside_bounds(float radius) {
    return engine::math::Vector(
        random_float(-bounds.get_x() + radius, bounds.get_x() - radius),
        random_float(-bounds.get_y() + radius, bounds.get_y() - radius),
        random_float(-bounds.get_z() + radius, bounds.get_z() - radius)
    );
}

float ParticleSimulationBase::clamp_temperature(float value) {
    return std::max(0.05f, value);
}

void ParticleSimulationBase::move_particle(BrownianParticle& particle, float delta_time) const {
    particle.previous_position = particle.position;
    particle.position = particle.position + particle.velocity * delta_time;
}

void ParticleSimulationBase::resolve_wall_collision(BrownianParticle& particle) const {
    resolve_wall_collision(particle, 1.0f);
}

void ParticleSimulationBase::resolve_wall_collision(
    BrownianParticle& particle,
    float wall_restitution
) const {
    float x = particle.position.get_x();
    float y = particle.position.get_y();
    float z = particle.position.get_z();

    float velocity_x = particle.velocity.get_x();
    float velocity_y = particle.velocity.get_y();
    float velocity_z = particle.velocity.get_z();

    clamp_axis(x, velocity_x, bounds.get_x() - particle.radius, wall_restitution);
    clamp_axis(y, velocity_y, bounds.get_y() - particle.radius, wall_restitution);
    clamp_axis(z, velocity_z, bounds.get_z() - particle.radius, wall_restitution);

    particle.position = engine::math::Vector(x, y, z);
    particle.velocity = engine::math::Vector(velocity_x, velocity_y, velocity_z);
}

void ParticleSimulationBase::resolve_particle_collision(
    BrownianParticle& first,
    BrownianParticle& second
) const {
    engine::math::Vector delta = second.position - first.position;
    float distance = delta.dist();
    const float collision_distance = first.radius + second.radius;

    if (distance >= collision_distance) {
        return;
    }

    engine::math::Vector normal(1.0f, 0.0f, 0.0f);
    if (distance > minimum_distance) {
        normal = delta * (1.0f / distance);
    } else {
        distance = minimum_distance;
    }

    const float inverse_first_mass = 1.0f / first.mass;
    const float inverse_second_mass = 1.0f / second.mass;
    const float inverse_mass_sum = inverse_first_mass + inverse_second_mass;
    if (inverse_mass_sum <= minimum_distance) {
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

    const float impulse_scalar = -(1.0f + restitution) * velocity_along_normal / inverse_mass_sum;
    const engine::math::Vector impulse = normal * impulse_scalar;

    first.velocity = first.velocity - impulse * inverse_first_mass;
    second.velocity = second.velocity + impulse * inverse_second_mass;
}

void ParticleSimulationBase::resolve_particle_collisions() {
    for (std::size_t first_index = 0; first_index < particles.size(); ++first_index) {
        for (std::size_t second_index = first_index + 1; second_index < particles.size(); ++second_index) {
            resolve_particle_collision(particles[first_index], particles[second_index]);
        }
    }
}
