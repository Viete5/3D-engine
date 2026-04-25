#include "../include/render/material.hpp"

#include "../include/render/shader.hpp"
#include "../include/render/texture.hpp"

namespace engine::render {

Material::Material(Shader& shader)
    : shader(&shader) {}

void Material::bind() const {
    shader->activate();

    shader->set_vec3("material.base_color", base_color);
    shader->set_int("material.has_base_color_texture", base_color_texture != nullptr);

    if (base_color_texture != nullptr) {
        shader->set_int("material.base_color_texture", 0);
        base_color_texture->bind_to_unit(0);
    }
}

Shader& Material::get_shader() {
    return *shader;
}

const Shader& Material::get_shader() const {
    return *shader;
}

engine::math::Vector Material::get_base_color() const {
    return base_color;
}

Texture* Material::get_base_color_texture() const {
    return base_color_texture;
}

void Material::set_base_color(const engine::math::Vector& new_base_color) {
    base_color = new_base_color;
}

void Material::set_base_color_texture(Texture* texture) {
    base_color_texture = texture;
}

} // namespace engine::render
