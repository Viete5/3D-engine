#ifndef MODEL_HPP
#define MODEL_HPP

#include "../scene/sceneObject.hpp"
#include "../scene/transform.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"

#include <cstddef>
#include <memory>
#include <vector>

namespace engine::render {

class Model final {
public:
    Model() = default;
    ~Model() = default;

    Model(const Model& other) = delete;
    Model& operator=(const Model& other) = delete;
    Model(Model&& other) noexcept = default;
    Model& operator=(Model&& other) noexcept = default;

    Mesh& create_mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    Material& create_material(Shader& shader);
    Texture& create_texture(const char* image_path, GLenum tex_type, GLenum slot, GLenum pixel_type);
    Texture& create_texture_from_memory(
        const unsigned char* data,
        int size,
        const char* debug_name,
        GLenum tex_type,
        GLenum slot,
        GLenum pixel_type
    );

    Mesh& add_mesh(std::unique_ptr<Mesh> mesh);
    Material& add_material(std::unique_ptr<Material> material);
    Texture& add_texture(std::unique_ptr<Texture> texture);
    engine::scene::SceneObject& add_object(const engine::scene::Transform& transform, Material& material, Mesh& mesh);

    Mesh& get_mesh(std::size_t index);
    Material& get_material(std::size_t index);
    Texture& get_texture(std::size_t index);

    const Mesh& get_mesh(std::size_t index) const;
    const Material& get_material(std::size_t index) const;
    const Texture& get_texture(std::size_t index) const;

    std::size_t get_mesh_count() const;
    std::size_t get_material_count() const;
    std::size_t get_texture_count() const;

    std::vector<engine::scene::SceneObject>& get_objects();
    const std::vector<engine::scene::SceneObject>& get_objects() const;

    void clear();

private:
    std::vector<std::unique_ptr<Mesh>> meshes;
    std::vector<std::unique_ptr<Material>> materials;
    std::vector<std::unique_ptr<Texture>> textures;
    std::vector<engine::scene::SceneObject> objects;
};

} // namespace engine::render

#endif // MODEL_HPP
