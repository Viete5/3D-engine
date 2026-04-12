#include "../include/core/application.hpp"
#include "../include/core/log.hpp"
#include "../include/core/time.hpp"
#include "../include/platform/window.hpp"
#include "../include/render/renderer.hpp"
#include "../include/scene/scene.hpp"

namespace engine::core {

Application::Application()
    : window(),
      renderer(),
      logger(),
      time(),
      scene(nullptr) {
}

int Application::run() {
    logger.add_sink(std::make_unique<ConsoleSink>());

    if (!initialize()) {
        logger.critical(LogCategory::Core, "application initialization failed");
        return -1;
    }
    logger.info(LogCategory::Core, "application started");

    while (!window.should_close()) {
        update_frame_time();

        window.poll_events();

        if (scene) {
            scene->update(time.delta_time);
            renderer.render(*scene);
        }
        
        window.swap_buffers();
    }

    logger.info(LogCategory::Core, "application shutting down");
    shutdown();
    return 0;
}

bool Application::initialize() {
    if (!window.create(800, 600, "My 3D Engine")) {
        return false;
    }

    scene = std::make_unique<scene::SandboxScene>();
    return true;
}

void Application::shutdown() {
    window.destroy();
}

void Application::update_frame_time() {
    time.update();
}

} // namespace engine::core