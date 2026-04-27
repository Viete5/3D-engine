#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <memory>
#include <string>

#include "../core/log.hpp"
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
};

class Application final {
public:
    Application();
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

    void set_scene(std::unique_ptr<engine::scene::Scene> new_scene);

    int run(const ApplicationConfig& config = ApplicationConfig{});

private:
    engine::platform::Window window;
    engine::platform::Input input;
    engine::render::Renderer renderer;
    Logger logger;
    Time time;

    std::unique_ptr<engine::scene::Scene> scene;

    bool initialize(const ApplicationConfig& config);
    void shutdown();
    void process_application_input();
};

} // namespace engine::core


#endif // APPLICATION_HPP