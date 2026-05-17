#include "../include/renderOpenGL/GLTexture.hpp"

#include "../include/assets/textureLoader.hpp"

#include <stdexcept>

namespace engine::render::opengl {

namespace {

void upload_texture(
    GLuint& id,
    GLenum& type,
    engine::assets::TextureLoader& tex_loader,
    GLenum tex_type,
    GLenum slot,
    GLenum pixel_type
) {
    if (!tex_loader.is_loaded()) {
        throw std::runtime_error(tex_loader.get_error_message());
    }

    type = tex_type;
    const GLenum external_format = (tex_loader.get_img_num_col_channel() == 4) ? GL_RGBA : GL_RGB;

    glGenTextures(1, &id);
    glActiveTexture(slot);
    glBindTexture(tex_type, id);

    glTexParameteri(tex_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(tex_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(tex_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(tex_type, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(
        tex_type,
        0,
        external_format,
        tex_loader.get_img_width(),
        tex_loader.get_img_height(),
        0,
        external_format,
        pixel_type,
        tex_loader.get_bytes()
    );
    glGenerateMipmap(tex_type);

    glBindTexture(tex_type, 0);
}

} // namespace

GLTexture::GLTexture(const char* image_path, GLenum tex_type, GLenum slot, GLenum pixel_type) {
    engine::assets::TextureLoader tex_loader(image_path);
    upload_texture(id, type, tex_loader, tex_type, slot, pixel_type);
}

GLTexture::GLTexture(
    const unsigned char* data,
    int size,
    const char* debug_name,
    GLenum tex_type,
    GLenum slot,
    GLenum pixel_type
) {
    engine::assets::TextureLoader tex_loader(data, size, debug_name);
    upload_texture(id, type, tex_loader, tex_type, slot, pixel_type);
}

GLTexture::GLTexture(GLTexture&& other) noexcept
    : id(other.id),
      type(other.type) {
    other.id = 0;
    other.type = 0;
}

GLTexture& GLTexture::operator=(GLTexture&& other) noexcept {
    if (&other == this) {
        return *this;
    }

    if (id != 0) {
        glDeleteTextures(1, &id);
    }

    id = other.id;
    other.id = 0;

    type = other.type;
    other.type = 0;

    return *this;
}

GLTexture::~GLTexture() {
    if (id != 0) {
        glDeleteTextures(1, &id);
    }
}

void GLTexture::bind() const {
    glBindTexture(type, id);
}

void GLTexture::unbind() const {
    glBindTexture(type, 0);
}

void GLTexture::bind_to_unit(GLenum slot) const {
    glActiveTexture(slot);
    glBindTexture(type, id);
}

} // namespace engine::render::opengl
