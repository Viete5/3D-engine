#ifndef EBO_HPP
#define EBO_HPP

#include <glad/glad.h>

#include <cstddef>

namespace engine::render::opengl {

class EBO {
public:
    EBO(const void* indices, std::size_t size, GLenum usage = GL_STATIC_DRAW);
    EBO();
    EBO(const EBO&) = delete;
    EBO& operator=(const EBO&) = delete;
    EBO(EBO&& other) noexcept;
    EBO& operator=(EBO&& other) noexcept;
    ~EBO();

    void bind() const;
    void unbind() const;
    void set_data(const void* indices, std::size_t size, GLenum usage = GL_STATIC_DRAW) const;

private:
    unsigned int id = 0;
};

} // namespace engine::render::opengl

#endif //EBO_HPP
