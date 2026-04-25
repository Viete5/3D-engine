#ifndef SCENE_OBJECT_HPP
#define SCENE_OBJECT_HPP

#include "../render/material.hpp"
#include "../render/mesh.hpp"
#include "transform.hpp"

namespace engine::scene {

class SceneObject {
public:
    SceneObject(const Transform& transform, engine::render::Material* material, engine::render::Mesh* mesh);

    Transform& get_transform();
    engine::render::Material& get_material();
    engine::render::Mesh& get_mesh();

    const Transform& get_transform() const;
    const engine::render::Material& get_material() const;
    const engine::render::Mesh& get_mesh() const;

    void set_transform(const Transform& new_transform);
    void set_material(engine::render::Material* new_material);
    void set_mesh(engine::render::Mesh* new_mesh);

private:
    Transform transform;
    engine::render::Material* material = nullptr;
    engine::render::Mesh* mesh = nullptr;
};


} // namespace engine::scene

#endif