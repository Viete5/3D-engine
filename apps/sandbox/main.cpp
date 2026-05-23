#include "../../engine/include/core/application.hpp"
#include "sandbox.hpp"

#include <filesystem>
#include <memory>

int main() {
    engine::core::Application app;

    app.set_scene_factory([](const engine::core::Paths& paths, engine::core::Logger& logger) {
        return std::make_unique<Sandbox>(paths, logger);
    });

    engine::core::ApplicationConfig config;
    config.assets_dir = std::filesystem::path(OPENGL_PROJECT_ROOT) / "assets";

    return app.run(config);
}
