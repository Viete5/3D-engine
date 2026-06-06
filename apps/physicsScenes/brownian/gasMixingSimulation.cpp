#include "gasMixingSimulation.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>

namespace {

constexpr int spawn_attempts = 96;

} // namespace

GasMixingSimulation::GasMixingSimulation()
    : GasMixingSimulation(GasMixingSimulationSettings{}) {
}

GasMixingSimulation::GasMixingSimulation(const GasMixingSimulationSettings& settings)
    : ParticleSimulationBase(settings.bounds, settings.initial_temperature, settings.restitution),
      settings(settings) {
    reset();
}

void GasMixingSimulation::reset() {
    temperature = clamp_temperature(settings.initial_temperature);
    valve_open = false;
    settings.gas_a_particle_count = std::clamp(
        settings.gas_a_particle_count,
        0,
        settings.max_particles_per_gas
    );
    settings.gas_b_particle_count = std::clamp(
        settings.gas_b_particle_count,
        0,
        settings.max_particles_per_gas
    );

    particles.clear();
    particles.reserve(
        static_cast<std::size_t>(settings.gas_a_particle_count + settings.gas_b_particle_count)
    );

    for (int index = 0; index < settings.gas_a_particle_count; ++index) {
        add_particle(BrownianGasType::GasA);
    }

    for (int index = 0; index < settings.gas_b_particle_count; ++index) {
        add_particle(BrownianGasType::GasB);
    }

    ++particle_revision;
}

void GasMixingSimulation::update(float delta_time) {
    if (paused || delta_time <= 0.0f) {
        return;
    }

    for (BrownianParticle& particle : particles) {
        move_particle(particle, delta_time);
        resolve_wall_collision(particle, restitution);
        resolve_partition_collision(particle);
    }

    resolve_particle_collisions();

    for (BrownianParticle& particle : particles) {
        resolve_wall_collision(particle, restitution);
        resolve_partition_collision(particle);
    }
}

void GasMixingSimulation::set_valve_open(bool value) {
    valve_open = value;
}

void GasMixingSimulation::toggle_valve() {
    valve_open = !valve_open;
}

void GasMixingSimulation::set_gas_particle_count(BrownianGasType gas_type, int new_count) {
    new_count = std::clamp(new_count, 0, settings.max_particles_per_gas);
    const int current_count = get_gas_particle_count(gas_type);
    if (new_count == current_count) {
        return;
    }

    if (gas_type == BrownianGasType::GasA) {
        settings.gas_a_particle_count = new_count;
    } else {
        settings.gas_b_particle_count = new_count;
    }

    if (new_count > current_count) {
        const int add_count = new_count - current_count;
        particles.reserve(particles.size() + static_cast<std::size_t>(add_count));

        for (int index = 0; index < add_count; ++index) {
            add_particle(gas_type);
        }
    } else {
        remove_particles(gas_type, current_count - new_count);
    }

    ++particle_revision;
}

void GasMixingSimulation::change_gas_particle_count(BrownianGasType gas_type, int delta) {
    set_gas_particle_count(gas_type, get_gas_particle_count(gas_type) + delta);
}

const GasMixingSimulationSettings& GasMixingSimulation::get_settings() const {
    return settings;
}

GasMixingStats GasMixingSimulation::get_gas_stats() const {
    GasMixingStats stats;

    for (const BrownianParticle& particle : particles) {
        const bool is_left = particle.position.get_x() < 0.0f;

        if (particle.gas_type == BrownianGasType::GasA) {
            ++stats.gas_a_total;
            is_left ? ++stats.gas_a_left : ++stats.gas_a_right;
        } else {
            ++stats.gas_b_total;
            is_left ? ++stats.gas_b_left : ++stats.gas_b_right;
        }
    }

    const int left_total = stats.gas_a_left + stats.gas_b_left;
    const int right_total = stats.gas_a_right + stats.gas_b_right;
    if (left_total > 0 && right_total > 0) {
        const float left_gas_a_ratio =
            static_cast<float>(stats.gas_a_left) / static_cast<float>(left_total);
        const float right_gas_a_ratio =
            static_cast<float>(stats.gas_a_right) / static_cast<float>(right_total);

        stats.mixing_factor = std::clamp(
            1.0f - std::abs(left_gas_a_ratio - right_gas_a_ratio),
            0.0f,
            1.0f
        );
    }

    return stats;
}

bool GasMixingSimulation::get_valve_open() const {
    return valve_open;
}

int GasMixingSimulation::get_gas_particle_count(BrownianGasType gas_type) const {
    return static_cast<int>(
        std::count_if(
            particles.begin(),
            particles.end(),
            [gas_type](const BrownianParticle& particle) {
                return particle.gas_type == gas_type;
            }
        )
    );
}

std::size_t GasMixingSimulation::get_particle_revision() const {
    return particle_revision;
}

BrownianParticle GasMixingSimulation::create_particle(BrownianGasType gas_type) {
    BrownianParticle particle;
    particle.gas_type = gas_type;
    particle.radius = random_particle_radius();
    particle.mass = get_particle_mass(particle.radius);
    particle.position = random_position_in_chamber(gas_type, particle.radius);
    particle.previous_position = particle.position;
    particle.velocity = random_direction() * (settings.base_speed * temperature);
    particle.color = gas_type == BrownianGasType::GasA
        ? engine::math::Vector(0.1f, 0.55f, 1.0f)
        : engine::math::Vector(1.0f, 0.28f, 0.12f);

    return particle;
}

void GasMixingSimulation::add_particle(BrownianGasType gas_type) {
    BrownianParticle particle = create_particle(gas_type);

    for (int attempt = 0; attempt < spawn_attempts; ++attempt) {
        if (!overlaps_existing_particles(particle)) {
            particles.push_back(particle);
            return;
        }

        particle.position = random_position_in_chamber(gas_type, particle.radius);
        particle.previous_position = particle.position;
    }

    particles.push_back(particle);
}

void GasMixingSimulation::remove_particles(BrownianGasType gas_type, int remove_count) {
    for (std::size_t index = particles.size(); index > 0 && remove_count > 0; --index) {
        if (particles[index - 1].gas_type == gas_type) {
            particles.erase(particles.begin() + static_cast<std::ptrdiff_t>(index - 1));
            --remove_count;
        }
    }
}

engine::math::Vector GasMixingSimulation::random_position_in_chamber(
    BrownianGasType gas_type,
    float radius
) {
    const float partition_margin = settings.partition_thickness * 0.5f + radius;

    float min_x = -bounds.get_x() + radius;
    float max_x = bounds.get_x() - radius;
    if (gas_type == BrownianGasType::GasA) {
        max_x = -partition_margin;
    } else {
        min_x = partition_margin;
    }

    return engine::math::Vector(
        random_float(min_x, max_x),
        random_float(-bounds.get_y() + radius, bounds.get_y() - radius),
        random_float(-bounds.get_z() + radius, bounds.get_z() - radius)
    );
}

float GasMixingSimulation::random_particle_radius() {
    return random_float(settings.min_radius, settings.max_radius);
}

float GasMixingSimulation::get_particle_mass(float radius) const {
    const float reference_radius = std::max(settings.min_radius, minimum_distance);
    const float radius_ratio = radius / reference_radius;
    return settings.base_mass * radius_ratio * radius_ratio * radius_ratio;
}

bool GasMixingSimulation::overlaps_existing_particles(const BrownianParticle& particle) const {
    for (const BrownianParticle& existing_particle : particles) {
        const float collision_distance = particle.radius + existing_particle.radius;
        if (particle.position.dist_between_dots(existing_particle.position) < collision_distance) {
            return true;
        }
    }

    return false;
}

void GasMixingSimulation::resolve_partition_collision(BrownianParticle& particle) const {
    if (valve_open) {
        return;
    }

    const float partition_limit = settings.partition_thickness * 0.5f + particle.radius;
    const float x = particle.position.get_x();
    const float previous_x = particle.previous_position.get_x();
    const bool crossed_from_left = previous_x <= -partition_limit && x > -partition_limit;
    const bool crossed_from_right = previous_x >= partition_limit && x < partition_limit;
    const bool inside_partition = std::abs(x) < partition_limit;

    if (!crossed_from_left && !crossed_from_right && !inside_partition) {
        return;
    }

    float velocity_x = particle.velocity.get_x();
    float new_x = x;

    if (crossed_from_left || (!crossed_from_right && velocity_x > 0.0f)) {
        new_x = -partition_limit;
        velocity_x = -std::abs(velocity_x) * restitution;
    } else if (crossed_from_right || velocity_x < 0.0f) {
        new_x = partition_limit;
        velocity_x = std::abs(velocity_x) * restitution;
    } else {
        new_x = x < 0.0f ? -partition_limit : partition_limit;
    }

    particle.position = engine::math::Vector(
        new_x,
        particle.position.get_y(),
        particle.position.get_z()
    );
    particle.velocity = engine::math::Vector(
        velocity_x,
        particle.velocity.get_y(),
        particle.velocity.get_z()
    );
}
