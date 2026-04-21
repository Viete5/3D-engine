#ifndef EBO_HPP
#define EBO_HPP

#include <glad/glad.h>

namespace engine::render::opengl {

class EBO {
public:
    EBO(const void *indices, size_t size);
    EBO();
    EBO(const EBO&) = delete;
    EBO& operator=(const EBO&) = delete;
    EBO(EBO&& other) noexcept;
    EBO& operator=(EBO&& other) noexcept;
    ~EBO();

    void bind() const;
    void unbind() const;

private:
    unsigned int id = 0;
};

} // namespace engine::render::opengl

#endif //EBO_HPP