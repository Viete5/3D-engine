#ifndef MESH_HPP
#define MESH_HPP

#include "../math/vector.hpp"
#include "../renderOpenGL/GLVertexArray.hpp"
#include "../renderOpenGL/GLIndexBuffer.hpp"
#include "../renderOpenGL/GLVertexBuffer.hpp"
#include <vector>

namespace engine::render {

struct Vertex {
    engine::math::Vector position;
    engine::math::Vector normal;
    float tex_u;
    float tex_v;
};


class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

    void draw() const;

private:
    opengl::VAO vao;
    opengl::VBO vbo;
    opengl::EBO ebo;
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};


} // namespace engine::render

#endif // MESH_HPP