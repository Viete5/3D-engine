#include "../include/render/shader.hpp"
#include "../include/render/texture.hpp"

namespace engine::render {

Texture::Texture(const char* image_path, GLenum tex_type, GLenum slot,  GLenum pixel_type) 
    : gl_texture(image_path, tex_type, slot, pixel_type) {}

void Texture::bind() const {
    gl_texture.bind();
}

void Texture::unbind() const {
    gl_texture.unbind();
}

void Texture::bind_to_unit(unsigned int unit) const {
    gl_texture.bind_to_unit(GL_TEXTURE0 + unit);
}



} // namespace engine::render