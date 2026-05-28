#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "../math/vector.hpp"
#include "../math/vector4.hpp"

namespace engine::render {

class Shader;
class Texture;

class Material {
public:
    Material(Shader& shader);

    void bind() const;

    Shader& get_shader();
    const Shader& get_shader() const;

    engine::math::Vector4 get_base_color() const;
    Texture* get_base_color_texture() const;
    bool is_transparent() const;

    void set_base_color(const engine::math::Vector& new_base_color);
    void set_base_color(const engine::math::Vector4& new_base_color);
    void set_alpha(float new_alpha);
    void set_base_color_texture(Texture* texture);

private:
    Shader* shader = nullptr;
    engine::math::Vector4 base_color = engine::math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    Texture* base_color_texture = nullptr;
};

} // namespace engine::render

#endif // MATERIAL_HPP
