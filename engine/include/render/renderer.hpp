#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>

namespace engine::scene {
class Scene;
class SceneObject;
class Camera;
class Light;
}

namespace engine::render {

class Renderer {
public:
    void render(const engine::scene::Scene& scene) const;
private:
    void setup_state() const;
    void clear() const;
    void sort_transparent_objects(std::vector<const engine::scene::SceneObject*>&,
                                  const engine::scene::Camera&) const;
    void draw_object(const engine::scene::SceneObject&, 
                     const engine::scene::Camera&, 
                     const std::vector<engine::scene::Light>&, 
                     const int lights_count) const;
}; 



} // namespace engine::render

#endif // RENDERER_HPP