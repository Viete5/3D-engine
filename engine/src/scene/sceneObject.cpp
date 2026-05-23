#include "../include/scene/sceneObject.hpp"

#include <stdexcept>
#include <utility>

namespace engine::scene {

SceneObject::SceneObject(
    const Transform& transform,
    engine::render::Material* material,
    engine::render::Mesh* mesh
)
    : SceneObject("", transform, material, mesh) {
}

SceneObject::SceneObject(
    std::string name,
    const Transform& transform,
    engine::render::Material* material,
    engine::render::Mesh* mesh
)
    : name(std::move(name)),
      transform(transform),
      material(material),
      mesh(mesh) {}

Transform& SceneObject::get_transform() {
    return transform;
}

engine::render::Material& SceneObject::get_material() {
    if (material == nullptr) {
        throw std::runtime_error("SceneObject material is null");
    }

    return *material;
}

engine::render::Mesh& SceneObject::get_mesh() {
    if (mesh == nullptr) {
        throw std::runtime_error("SceneObject mesh is null");
    }

    return *mesh;
}

const std::string& SceneObject::get_name() const {
    return name;
}

const Transform& SceneObject::get_transform() const {
    return transform;
}

const engine::render::Material& SceneObject::get_material() const {
    if (material == nullptr) {
        throw std::runtime_error("SceneObject material is null");
    }

    return *material;
}

const engine::render::Mesh& SceneObject::get_mesh() const {
    if (mesh == nullptr) {
        throw std::runtime_error("SceneObject mesh is null");
    }

    return *mesh;
}

void SceneObject::set_name(std::string new_name) {
    name = std::move(new_name);
}

void SceneObject::set_transform(const Transform& new_transform) {
    transform = new_transform;
}

void SceneObject::set_material(engine::render::Material* new_material) {
    material = new_material;
}

void SceneObject::set_mesh(engine::render::Mesh* new_mesh) {
    mesh = new_mesh;
}

} // namespace engine::scene
