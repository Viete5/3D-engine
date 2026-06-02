#include "../include/scene/scene.hpp"

namespace engine::scene {

Scene::Scene(const std::vector<SceneObject>& objects, const Camera& camera, const std::vector<Light>& lights) 
    : objects(objects),
      camera(camera),
      lights(lights) {}

const std::vector<SceneObject>& Scene::get_objects() const {
    return objects;
}

std::vector<SceneObject>& Scene::get_objects() {
    return objects;
}

const Camera& Scene::get_camera() const {
    return camera;
}

Camera& Scene::get_camera() {
    return camera;
}

const std::vector<Light>& Scene::get_lights() const {
    return lights;
}

std::vector<Light>& Scene::get_lights() {
    return lights;
}

void Scene::set_objects(const std::vector<SceneObject>& new_objects) {
    objects = new_objects;
}

void Scene::set_camera(const Camera& new_camera) {
    camera = new_camera;
}

void Scene::set_lights(const std::vector<Light>& new_lights) {
    lights = new_lights;
}

void Scene::render_ui(
    engine::render::TextRenderer& text_renderer,
    const engine::render::Font& font,
    unsigned int viewport_width,
    unsigned int viewport_height
) const {
    (void)text_renderer;
    (void)font;
    (void)viewport_width;
    (void)viewport_height;
}

} // namespace engine::scene
