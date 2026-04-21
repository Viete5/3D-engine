#ifndef VBO_HPP
#define VBO_HPP

#include <glad/glad.h>

namespace engine::render::opengl {

class VBO {
public:
    VBO(const void *vertices, size_t size);
    VBO();
    VBO(const VBO&) = delete;
    VBO& operator=(const VBO&) = delete;
    VBO(VBO&& other) noexcept;
    VBO& operator=(VBO&& other) noexcept;
    ~VBO();

    void bind() const;
    void unbind() const;

private:
    unsigned int id = 0;
};

} // namespace engine::render::opengl

#endif //VBO_HPP