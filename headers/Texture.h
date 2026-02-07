#ifndef TEXTURE_H
#define TEXTURE_H
#include <glad/glad.h>
#include "Shader.h"

class Texture
{
public:
    GLuint ID; // ID of Texture object
    GLenum type; // Texture type (GL_TEXTURE_2D)

    Texture(const char* imagePath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
    ~Texture();

    void texUnit(Shader& shader, const char* uniform, GLuint unit) const;
    
    void Bind() const;
    
    void Unbind() const;
    
    void Delete();
};

#endif //TEXTURE_H