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

    vao.link_attrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex,position)));
    vao.link_attrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex,normal)));
    vao.link_attrib(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex,tex_u)));

    vao.unbind();
    vbo.unbind();
    ebo.unbind();
}

void Mesh::draw() const {
    vao.bind();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    vao.unbind();
}





} // namespace engine::render