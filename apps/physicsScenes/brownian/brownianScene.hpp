#ifndef BROWNIAN_SCENE_HPP
#define BROWNIAN_SCENE_HPP

#include "brownianSimulation.hpp"

#include "../../../engine/include/render/material.hpp"
#include "../../../engine/include/render/mesh.hpp"
#include "../../../engine/include/render/shader.hpp"
#include "../../../engine/include/scene/scene.hpp"

#include <cstddef>

namespace engine::core {
class Logger;
class Paths;
}

class BrownianScene final : public engine::scene::Scene {
public:
    BrownianScene(const engine::core::Paths& paths, engine::core::Logger& logger);

    void update(const engine::scene::SceneUpdateContext& context) override;

private:
    void create_particle_objects();
    void sync_particle_objects();
    void handle_camera_input(const engine::scene::SceneUpdateContext& context);
    void handle_simulation_input(const engine::scene::SceneUpdateContext& context);

    BrownianSimulation simulation;

    engine::render::Shader shader;
    engine::render::Mesh particle_mesh;
    engine::render::Material small_particle_material;
    engine::render::Material large_particle_material;

    std::size_t large_particle_object_index = 0;
};

#endif // BROWNIAN_SCENE_HPP