#ifndef RENDERER_HPP
#define RENDERER_HPP

namespace engine::scene {
class Scene;
}

namespace engine::render {

class Renderer {
public:
    void render(const engine::scene::Scene& scene) const;
}; 



} // namespace engine::render

#endif // RENDERER_HPP