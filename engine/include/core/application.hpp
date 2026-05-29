#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <memory>
#include <string>
#include <functional>
#include <filesystem>

#include "../core/log.hpp"
#include "../core/paths.hpp"
#include "../core/time.hpp"
#include "../platform/input.hpp"
#include "../platform/window.hpp"
#include "../render/renderer.hpp"

namespace engine::scene {
class Scene;
}

namespace engine::core {

struct ApplicationConfig {
    std::string title = "My 3D Engine";
    unsigned int width = 800;
    unsigned int height = 600;
    std::filesystem::path assets_dir;
};

class Application final {
public:
    using SceneFactory = std::function<std::unique_ptr<engine::scene::Scene>(const Paths& paths, Logger& logger)>;

    Application();
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

    void set_scene(std::unique_ptr<engine::scene::Scene> new_scene);
    void set_scene_factory(SceneFactory new_scene_factory);

    int run(const ApplicationConfig& config = ApplicationConfig{});

private:
    engine::platform::Window window;
    engine::platform::Input input;
    engine::render::Renderer renderer;
    Logger logger;
    Time time;
    Paths paths;

    std::unique_ptr<engine::scene::Scene> scene;
    SceneFactory scene_factory;

    bool initialize(const ApplicationConfig& config);
    void shutdown();
    void process_application_input();
};

} // namespace engine::core


#endif // APPLICATION_HPP
