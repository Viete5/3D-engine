#include "../include/render/mesh.hpp"
#include <cstddef>

namespace engine::render {

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) 
    : vertices(vertices),
      indices(indices),
      vao(),
      vbo(vertices.data(), vertices.size()*sizeof(Vertex)),
      ebo(indices.data(), indices.size() * sizeof(unsigned int)) {

    vao.bind();
    ebo.bind();
    link_vertex_attributes();

    vao.unbind();
    vbo.unbind();
    ebo.unbind();
}

void Mesh::draw() const {
    if (indices.empty()) {
        return;
    }

    vao.bind();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    vao.unbind();
}

void Mesh::update_data(
    const std::vector<Vertex>& new_vertices,
    const std::vector<unsigned int>& new_indices
) {
    vertices = new_vertices;
    indices = new_indices;

    vbo.set_data(vertices.empty() ? nullptr : vertices.data(), vertices.size() * sizeof(Vertex), GL_DYNAMIC_DRAW);

    vao.bind();
    ebo.set_data(indices.empty() ? nullptr : indices.data(), indices.size() * sizeof(unsigned int), GL_DYNAMIC_DRAW);
    vao.unbind();
}

void Mesh::link_vertex_attributes() {
    vao.link_attrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex,position)));
    vao.link_attrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex,normal)));
    vao.link_attrib(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex,tex_u)));
    vao.link_attrib(vbo, 3, 4, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex,color)));
}





} // namespace engine::render
