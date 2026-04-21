#include "../include/renderOpenGL/GLVertexBuffer.hpp"

namespace engine::render::opengl {

VBO::VBO(const void *vertices, size_t size) {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VBO::VBO() {
    glGenBuffers(1, &id);
}

VBO::VBO(VBO&& other) noexcept
    : id(other.id) {
    other.id = 0;
}

VBO& VBO::operator=(VBO&& other) noexcept{
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

VBO::~VBO() {
    if (id != 0) {
        glDeleteBuffers(1, &id);
    }
}

void VBO::bind() const{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::unbind() const{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace engine::render::opengl