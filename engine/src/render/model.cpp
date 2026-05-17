#include "../include/render/model.hpp"

#include <stdexcept>
#include <utility>

namespace engine::render {

Mesh& Model::create_mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    return add_mesh(std::make_unique<Mesh>(vertices, indices));
}

Material& Model::create_material(Shader& shader) {
    return add_material(std::make_unique<Material>(shader));
}

Texture& Model::create_texture(const char* image_path, GLenum tex_type, GLenum slot, GLenum pixel_type) {
    return add_texture(std::make_unique<Texture>(image_path, tex_type, slot, pixel_type));
}

Texture& Model::create_texture_from_memory(
    const unsigned char* data,
    int size,
    const char* debug_name,
    GLenum tex_type,
    GLenum slot,
    GLenum pixel_type
) {
    return add_texture(std::make_unique<Texture>(data, size, debug_name, tex_type, slot, pixel_type));
}

Mesh& Model::add_mesh(std::unique_ptr<Mesh> mesh) {
    if (!mesh) {
        throw std::runtime_error("Model mesh is null");
    }

    meshes.push_back(std::move(mesh));
    return *meshes.back();
}

Material& Model::add_material(std::unique_ptr<Material> material) {
    if (!material) {
        throw std::runtime_error("Model material is null");
    }

    materials.push_back(std::move(material));
    return *materials.back();
}

Texture& Model::add_texture(std::unique_ptr<Texture> texture) {
    if (!texture) {
        throw std::runtime_error("Model texture is null");
    }

    textures.push_back(std::move(texture));
    return *textures.back();
}

engine::scene::SceneObject& Model::add_object(const engine::scene::Transform& transform, Material& material, Mesh& mesh) {
    objects.emplace_back(transform, &material, &mesh);
    return objects.back();
}

Mesh& Model::get_mesh(std::size_t index) {
    if (index >= meshes.size()) {
        throw std::out_of_range("Model mesh index is out of range");
    }

    return *meshes[index];
}

Material& Model::get_material(std::size_t index) {
    if (index >= materials.size()) {
        throw std::out_of_range("Model material index is out of range");
    }

    return *materials[index];
}

Texture& Model::get_texture(std::size_t index) {
    if (index >= textures.size()) {
        throw std::out_of_range("Model texture index is out of range");
    }

    return *textures[index];
}

const Mesh& Model::get_mesh(std::size_t index) const {
    if (index >= meshes.size()) {
        throw std::out_of_range("Model mesh index is out of range");
    }

    return *meshes[index];
}

const Material& Model::get_material(std::size_t index) const {
    if (index >= materials.size()) {
        throw std::out_of_range("Model material index is out of range");
    }

    return *materials[index];
}

const Texture& Model::get_texture(std::size_t index) const {
    if (index >= textures.size()) {
        throw std::out_of_range("Model texture index is out of range");
    }

    return *textures[index];
}

std::size_t Model::get_mesh_count() const {
    return meshes.size();
}

std::size_t Model::get_material_count() const {
    return materials.size();
}

std::size_t Model::get_texture_count() const {
    return textures.size();
}

std::vector<engine::scene::SceneObject>& Model::get_objects() {
    return objects;
}

const std::vector<engine::scene::SceneObject>& Model::get_objects() const {
    return objects;
}

void Model::clear() {
    objects.clear();
    textures.clear();
    materials.clear();
    meshes.clear();
}

} // namespace engine::render
