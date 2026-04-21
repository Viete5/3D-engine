#ifndef VAO_HPP
#define VAO_HPP

#include <glad/glad.h>
#include "GLVertexBuffer.hpp"

namespace engine::render::opengl {

class VAO {
public:
    VAO();
    VAO(const VAO&) = delete;
    VAO& operator=(const VAO&) = delete;
    VAO(VAO&& other) noexcept;
    VAO& operator=(VAO&& other) noexcept;
    ~VAO();

    void link_attrib(VBO& VBO, unsigned int layout, unsigned int numComponents, GLenum type, GLsizei stride, const void* offset) const;
    void bind() const;
    void unbind() const;

private:
    unsigned int id = 0;
};

} // namespace engine::render::opengl

#endif //VAO_HPP