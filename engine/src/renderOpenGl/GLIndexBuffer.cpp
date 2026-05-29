#include "../include/renderOpenGL/GLIndexBuffer.hpp"

namespace engine::render::opengl {

EBO::EBO(const void* indices, std::size_t size, GLenum usage) {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), indices, usage);
}

EBO::EBO() {
    glGenBuffers(1, &id);
}

EBO::EBO(EBO&& other) noexcept
    : id(other.id) {
    other.id = 0;
}

EBO& EBO::operator=(EBO&& other) noexcept {
    if (&other == this) {
        return *this;
    }

    if (id != 0) {
        glDeleteBuffers(1, &id);
    }

    id = other.id;
    other.id = 0;

    return *this;
}

EBO::~EBO() {
    if (id != 0) {
        glDeleteBuffers(1, &id);
    }
}

void EBO::bind() const{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void EBO::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::set_data(const void* indices, std::size_t size, GLenum usage) const {
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), indices, usage);
}

} // namespace engine::render::opengl
