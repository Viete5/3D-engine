#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "../include/renderOpenGL/GLTexture.hpp"

namespace engine::render {

class Shader;

class Texture {
public:
    Texture(const char* image_path, GLenum tex_type, GLenum slot,  GLenum pixel_type);
    Texture(const Texture& other) = delete;
    Texture& operator=(const Texture& other) = delete;
    Texture(Texture&& other) noexcept = default;
    Texture& operator=(Texture&& other) noexcept = default;
    ~Texture() = default;

    void bind() const;
    void unbind() const;
    void bind_to_unit(unsigned int unit) const;

private:
    opengl::GLTexture gl_texture;
};

} // namepace engine::render

#endif // TEXTURE_HPP