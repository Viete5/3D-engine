#ifndef BROWNIAN_SCENE_HPP
#define BROWNIAN_SCENE_HPP

#include "brownianSimulation.hpp"
#include "brownianTrail.hpp"
#include "gasMixingSimulation.hpp"
#include "largeParticleBrownianSimulation.hpp"

#include "../../../engine/include/render/material.hpp"
#include "../../../engine/include/render/mesh.hpp"
#include "../../../engine/include/render/shader.hpp"
#include "../../../engine/include/scene/scene.hpp"

#include <cstddef>
#include <string>

namespace engine::core {
class Logger;
class Paths;
}

class BrownianScene final : public engine::scene::Scene {
public:
    BrownianScene(const engine::core::Paths& paths, engine::core::Logger& logger);

    void update(const engine::scene::SceneUpdateContext& context) override;
    void render_ui(
        engine::render::TextRenderer& text_renderer,
        const engine::render::Font& font,
        unsigned int viewport_width,
        unsigned int viewport_height
    ) const override;

private:
    enum class Mode {
        LargeParticle,
        GasMixing
    };

    void set_mode(Mode new_mode, float elapsed_time);
    void rebuild_scene_objects();

    void create_large_particle_objects();
    void create_gas_particle_objects();
    void create_container_object(const engine::math::Vector& bounds, const std::string& name);
    void create_partition_object();
    void create_trail_object();

    void sync_large_particle_objects();
    void sync_gas_particle_objects();
    void update_partition_visual();
    void update_fps(float delta_time);

    void handle_camera_input(const engine::scene::SceneUpdateContext& context);
    void handle_simulation_input(const engine::scene::SceneUpdateContext& context);
    void handle_large_particle_input(const engine::scene::SceneUpdateContext& context);
    void handle_gas_mixing_input(const engine::scene::SceneUpdateContext& context);

    void render_large_particle_ui(
        engine::render::TextRenderer& text_renderer,
        const engine::render::Font& font,
        unsigned int viewport_width,
        unsigned int viewport_height
    ) const;
    void render_gas_mixing_ui(
        engine::render::TextRenderer& text_renderer,
        const engine::render::Font& font,
        unsigned int viewport_width,
        unsigned int viewport_height
    ) const;

    engine::render::Material* get_gas_particle_material(BrownianGasType gas_type);

    Mode mode = Mode::LargeParticle;
    LargeParticleBrownianSimulation large_particle_simulation;
    GasMixingSimulation gas_mixing_simulation;

    engine::render::Shader shader;
    engine::render::Mesh particle_mesh;
    engine::render::Mesh container_mesh;
    BrownianTrail large_particle_trail;

    engine::render::Material small_particle_material;
    engine::render::Material large_particle_material;
    engine::render::Material attractor_material;
    engine::render::Material gas_a_material;
    engine::render::Material gas_b_material;
    engine::render::Material container_material;
    engine::render::Material partition_material;
    engine::render::Material trail_material;

    std::size_t particle_object_count = 0;
    std::size_t large_particle_object_index = 0;
    std::size_t attractor_object_index = 0;
    std::size_t synced_large_particle_revision = 0;
    std::size_t synced_attractor_revision = 0;
    std::size_t synced_gas_particle_revision = 0;

    float current_fps = 0.0f;
    float fps_time_accumulator = 0.0f;
    int fps_frame_count = 0;
};

#endif // BROWNIAN_SCENE_HPP
