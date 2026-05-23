#ifndef SANDBOX_HPP
#define SANDBOX_HPP

#include "../../engine/include/render/model.hpp"
#include "../../engine/include/render/shader.hpp"
#include "../../engine/include/scene/scene.hpp"

namespace engine::core {
class Paths;
class Logger;
}

class Sandbox final : public engine::scene::Scene {
public:
    Sandbox(const engine::core::Paths& paths, engine::core::Logger& logger);

    void update(const engine::scene::SceneUpdateContext& context) override;

private:
    void handle_camera_input(const engine::scene::SceneUpdateContext& context);

    engine::render::Shader shader;
    engine::render::Model tiger_model;
};

#endif // SANDBOX_HPP
