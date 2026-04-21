#include "../include/renderOpenGL/GLVertexArray.hpp"

namespace engine::render::opengl {

VAO::VAO() {
    glGenVertexArrays(1, &id);
}

VAO::VAO(VAO&& other)  noexcept
    : id(other.id) {
    other.id = 0;
}

VAO& VAO::operator=(VAO&& other) noexcept {
    if (&other == this) {
        return *this;
    }

    if (id!=0) {
        glDeleteVertexArrays(1, &id);
    }

    id = other.id;
    other.id = 0;

    return *this;
}

VAO::~VAO() {
    if (id!=0) {
        glDeleteVertexArrays(1, &id);
    }
}

void VAO::link_attrib(VBO& VBO, unsigned int layout, unsigned int numComponents, GLenum type, GLsizei stride, const void* offset) const{
    VBO.bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    VBO.unbind();
}

void VAO::bind() const {
    glBindVertexArray(id);
}

void VAO::unbind() const{
    glBindVertexArray(0);
}

} // namespace engine::render::opengl