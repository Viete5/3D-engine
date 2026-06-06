#ifndef GAS_MIXING_SIMULATION_HPP
#define GAS_MIXING_SIMULATION_HPP

#include "brownianSimulation.hpp"

#include "../../../engine/include/math/vector.hpp"

#include <cstddef>

struct GasMixingStats {
    int gas_a_total = 0;
    int gas_b_total = 0;
    int gas_a_left = 0;
    int gas_a_right = 0;
    int gas_b_left = 0;
    int gas_b_right = 0;
    float mixing_factor = 0.0f;
};

struct GasMixingSimulationSettings {
    int gas_a_particle_count = 220;
    int gas_b_particle_count = 220;
    int max_particles_per_gas = 500;
    int density_step = 25;

    engine::math::Vector bounds = engine::math::Vector(3.6f, 2.25f, 2.25f);

    float min_radius = 0.035f;
    float max_radius = 0.085f;
    float base_mass = 1.0f;
    float base_speed = 2.15f;
    float initial_temperature = 1.15f;
    float restitution = 0.98f;
    float partition_thickness = 0.05f;
};

class GasMixingSimulation : public ParticleSimulationBase {
public:
    GasMixingSimulation();
    GasMixingSimulation(const GasMixingSimulationSettings& settings);

    void update(float delta_time) override;
    void reset() override;

    void set_valve_open(bool value);
    void toggle_valve();

    void set_gas_particle_count(BrownianGasType gas_type, int new_count);
    void change_gas_particle_count(BrownianGasType gas_type, int delta);

    const GasMixingSimulationSettings& get_settings() const;
    GasMixingStats get_gas_stats() const;
    bool get_valve_open() const;
    int get_gas_particle_count(BrownianGasType gas_type) const;
    std::size_t get_particle_revision() const;

private:
    GasMixingSimulationSettings settings;
    bool valve_open = false;
    std::size_t particle_revision = 0;

    BrownianParticle create_particle(BrownianGasType gas_type);
    void add_particle(BrownianGasType gas_type);
    void remove_particles(BrownianGasType gas_type, int remove_count);

    engine::math::Vector random_position_in_chamber(BrownianGasType gas_type, float radius);
    float random_particle_radius();
    float get_particle_mass(float radius) const;
    bool overlaps_existing_particles(const BrownianParticle& particle) const;

    void resolve_partition_collision(BrownianParticle& particle) const;
};

#endif // GAS_MIXING_SIMULATION_HPP
