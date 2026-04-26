#include "../include/render/renderer.hpp"
#include "../include/scene/scene.hpp"
#include "../include/render/material.hpp"
#include "../include/render/shader.hpp"

#include <glad/glad.h>

#include <algorithm>
#include <string>

namespace engine::render {

void Renderer::render(const engine::scene::Scene& scene) const {
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const engine::scene::Camera& camera = scene.get_camera();
    const std::vector<engine::scene::Light>& lights = scene.get_lights();
    const int lights_count = static_cast<int>(std::min<std::size_t>(lights.size(), 16));
    
    for (const engine::scene::SceneObject& current_object : scene.get_objects()) {

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
}







} // namespace engine::render
