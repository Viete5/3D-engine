#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <memory>

namespace engine::platform {
class Window;
}

namespace engine::scene {
class Scene;
}

namespace engine::render {
class Renderer;
}

namespace engine::core {

class Logger;
class Time;

class Application final {
public:
    Application();
    ~Application() = default;

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    int run();

private:
    engine::platform::Window window;
    engine::render::Renderer renderer;
    Logger logger;
    Time time;

    std::unique_ptr<engine::scene::Scene> scene;

    bool initialize();
    void shutdown();
    void update_frame_time();
};

} // namespace engine::core


#endif // ENGINE_CORE_APPLICATION_HPP