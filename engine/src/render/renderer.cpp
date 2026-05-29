#include "../include/render/renderer.hpp"
#include "../include/scene/scene.hpp"
#include "../include/render/material.hpp"
#include "../include/render/shader.hpp"

#include <glad/glad.h>

#include <algorithm>
#include <string>

namespace engine::render {

void Renderer::render(const engine::scene::Scene& scene) const {
    setup_state();
    clear();

    const engine::scene::Camera& camera = scene.get_camera();
    const std::vector<engine::scene::Light>& lights = scene.get_lights();
    const int lights_count = static_cast<int>(std::min<std::size_t>(lights.size(), 16));
    
    std::vector<const engine::scene::SceneObject*> transparent_objects;

    for (const engine::scene::SceneObject& current_object : scene.get_objects()) {
        if (current_object.get_material().is_transparent()) {
            transparent_objects.push_back(&current_object);
            continue;
        }
        draw_object(current_object, camera, lights, lights_count);
    }

    sort_transparent_objects(transparent_objects, camera);

    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    for (const engine::scene::SceneObject* current_object : transparent_objects) {
        draw_object(*current_object, camera, lights, lights_count);
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
}

void Renderer::setup_state() const {
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void Renderer::clear() const {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::sort_transparent_objects(
                std::vector<const engine::scene::SceneObject*>& transparent_objects, 
                const engine::scene::Camera& camera) const {
    std::stable_sort(
        transparent_objects.begin(),
        transparent_objects.end(),
        [&camera](const auto* first, const auto* second) {
            const float first_distance =
                first->get_transform().get_position().dist_between_dots(camera.get_position());

            const float second_distance =
                second->get_transform().get_position().dist_between_dots(camera.get_position());

            return first_distance > second_distance;
        }
    );
}


void Renderer::draw_object(const engine::scene::SceneObject& current_object, 
                     const engine::scene::Camera& camera, 
                     const std::vector<engine::scene::Light>& lights, 
                     const int lights_count) const {

    const engine::render::Material& current_material = current_object.get_material();
    current_material.bind();

    const engine::render::Shader& current_shader = current_material.get_shader();
    current_shader.set_mat4("model", current_object.get_transform().get_model_matrix());
    current_shader.set_mat4("normal_matrix", current_object.get_transform().get_normal_matrix());
    current_shader.set_mat4("projection", camera.get_projection_matrix());
    current_shader.set_mat4("view", camera.get_view_matrix());
    current_shader.set_int("light_count", lights_count);

    for (int light_index = 0; light_index < lights_count; ++light_index) {
        const engine::scene::Light& current_light = lights[light_index];
        const std::string light_prefix = "lights[" + std::to_string(light_index) + "]";

        current_shader.set_vec3((light_prefix + ".position").c_str(), current_light.get_position());
        current_shader.set_vec3((light_prefix + ".color").c_str(), current_light.get_color());
    }

    current_object.get_mesh().draw();
}




} // namespace engine::render
