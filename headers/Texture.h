#ifndef TEXTURE_H
#define TEXTURE_H
#include <glad/glad.h>
#include "Shader.h" // Нужен для SetUniform1i

class Texture
{
public:
    GLuint ID; // ID текстурного объекта OpenGL
    GLenum type; // Тип текстуры (GL_TEXTURE_2D)

    // Конструктор: принимает путь к файлу и тип текстуры
    Texture(const char* imagePath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

    // Устанавливает текстурный юнит (вызывается один раз после активации шейдера)
    void texUnit(Shader& shader, const char* uniform, GLuint unit);
    
    // Привязывает текстуру перед рисованием
    void Bind();
    
    // Отвязывает текстуру
    void Unbind();
    
    // Удаляет текстуру (в конце main)
    void Delete();
};

#endif //TEXTURE_H