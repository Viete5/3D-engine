#ifndef VBO_HPP
#define VBO_HPP

#include <glad/glad.h>

#include <cstddef>

namespace engine::render::opengl {

class VBO {
public:
    VBO(const void* vertices, std::size_t size, GLenum usage = GL_STATIC_DRAW);
    VBO();
    VBO(const VBO&) = delete;
    VBO& operator=(const VBO&) = delete;
    VBO(VBO&& other) noexcept;
    VBO& operator=(VBO&& other) noexcept;
    ~VBO();

    void bind() const;
    void unbind() const;
    void set_data(const void* vertices, std::size_t size, GLenum usage = GL_STATIC_DRAW) const;

private:
    unsigned int id = 0;
};

} // namespace engine::render::opengl

#endif //VBO_HPP
