#include "../include/core/application.hpp"
#include "../include/core/log.hpp"
#include "../include/core/paths.hpp"
#include "../include/core/time.hpp"
#include "../include/assets/fontLoader.hpp"
#include "../include/platform/input.hpp"
#include "../include/platform/window.hpp"
#include "../include/render/renderer.hpp"
#include "../include/render/textRenderer.hpp"
#include "../include/scene/scene.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <exception>
#include <memory>
#include <utility>

namespace engine::core {

Application::Application()
    : window(),
      input(),
      renderer(),
      text_renderer(nullptr),
      ui_font(nullptr),
      logger(),
      time(),
      paths(),
      scene(nullptr) {
}

Application::~Application() = default;

void Application::set_scene(std::unique_ptr<engine::scene::Scene> new_scene) {
    scene = std::move(new_scene);
}

void Application::set_scene_factory(SceneFactory new_scene_factory) {
    scene_factory = std::move(new_scene_factory);
}

int Application::run(const ApplicationConfig& config) {
    logger.add_sink(std::make_unique<ConsoleSink>());

    try {
        if (!initialize(config)) {
            logger.critical(LogCategory::Core, "application initialization failed");
            shutdown();
            return -1;
        }

        if (!scene && scene_factory) {
            scene = scene_factory(paths, logger);
        }

        if (!scene) {
            logger.critical(LogCategory::Core, "application has no active scene");
            shutdown();
            return -1;
        }

        logger.info(LogCategory::Core, "application started");

        while (!window.should_close()) {
            time.update();
            input.begin_frame();
            window.poll_events();
            input.update_key_states();
            process_application_input();
            scene->get_camera().update_aspect_ratio(window.get_aspect_ratio());

            const engine::scene::SceneUpdateContext context{
                time.get_delta_time(),
                time.get_elapsed_time(),
                input
            };

            scene->update(context);
            renderer.render(*scene);
            if (text_renderer && ui_font) {
                scene->render_ui(*text_renderer, *ui_font, window.get_width(), window.get_height());
            }

            window.swap_buffers();
        }

        logger.info(LogCategory::Core, "application shutting down");
        shutdown();
        return 0;
    } catch (const std::exception& error) {
        logger.critical(LogCategory::Core, error.what());
        shutdown();
        return -1;
    } catch (...) {
        logger.critical(LogCategory::Core, "unknown application error");
        shutdown();
        return -1;
    }
}

bool Application::initialize(const ApplicationConfig& config) {
    paths.set_assets_dir(config.assets_dir);

    if (!glfwInit()) {
        logger.critical(LogCategory::Platform, "failed to initialize GLFW");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!window.create(config.title.c_str(), config.width, config.height)) {
        logger.critical(LogCategory::Platform, "failed to create window");
        return false;
    }

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        logger.critical(LogCategory::Render, "failed to initialize GLAD");
        return false;
    }

    input.attach(window.get_window());
    window.set_cursor_disabled(true);

    text_renderer = std::make_unique<engine::render::TextRenderer>(
        paths.get_shader_path("text_msdf.vs").string().c_str(),
        paths.get_shader_path("text_msdf.fs").string().c_str()
    );

    engine::assets::FontLoader font_loader;
    ui_font = std::make_unique<engine::render::Font>(
        font_loader.load_msdf_font(
            paths.get_font_path("nunito/nunito_regular_msdf.png"),
            paths.get_font_path("nunito/nunito_regular_msdf.json")
        )
    );

    return true;
}

void Application::shutdown() {
    window.destroy();
    glfwTerminate();
}

void Application::process_application_input() {
    if (input.is_key_down(engine::platform::Key::Escape)) {
        window.set_should_close(true);
    }
    if (input.is_key_pressed(engine::platform::Key::F11)) {
        window.toggle_fullscreen();
    }
}

} // namespace engine::core
