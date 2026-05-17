#ifndef GLTEXTURE_HPP
#define GLTEXTURE_HPP
#include <glad/glad.h>

namespace engine::render::opengl {

class GLShader;

class GLTexture
{
public:
    GLTexture(const char* image_path, GLenum tex_type, GLenum slot,  GLenum pixel_type);
    GLTexture(const unsigned char* data, int size, const char* debug_name, GLenum tex_type, GLenum slot, GLenum pixel_type);
    GLTexture(const GLTexture& other) = delete;
    GLTexture& operator=(const GLTexture& other) = delete;
    GLTexture(GLTexture&& other) noexcept;
    GLTexture& operator=(GLTexture&& other) noexcept;
    ~GLTexture();

    void bind_to_unit(GLenum slot) const;
    void bind() const;
    void unbind() const;
private:
    GLuint id = 0; // ID of Texture object
    GLenum type = 0; // Texture type
};

} // engine::render::opengl

#endif //GLTEXTURE_HPP
