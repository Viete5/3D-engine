#include "C:\prog\C++\openGL\headers\Texture.h"
#include <C:\prog\C++\openGL\vendor\include\stb\stb_image.h> // Предполагая, что stb_image находится здесь
#include <iostream>

// ВАЖНО: Устанавливаем переворот при загрузке один раз
void setTextureFlip() {
    stbi_set_flip_vertically_on_load(true);
}

Texture::Texture(const char* imagePath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
    // Вызываем setTextureFlip() один раз, если не уверены, что вызвали ее в main
    // setTextureFlip(); 
    
    type = texType;
    int widthImg, heightImg, numColCh;
    
    unsigned char* bytes = stbi_load(imagePath, &widthImg, &heightImg, &numColCh, 0);

    if (bytes)
    {
        // Динамическое определение формата (RGB или RGBA)
        GLenum externalFormat = (numColCh == 4) ? GL_RGBA : GL_RGB;
        
        glGenTextures(1, &ID);
        glActiveTexture(slot); // Активируем нужный слот
        glBindTexture(texType, ID);

        // Установка параметров (вы можете сделать их настраиваемыми)
        glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Загрузка изображения в GPU
        glTexImage2D(texType, 0, externalFormat, widthImg, heightImg, 0, externalFormat, pixelType, bytes);
        glGenerateMipmap(texType);
    }
    else
    {
        std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD_IMAGE (" << imagePath << ")\n" 
                  << stbi_failure_reason() << std::endl;
    }

    stbi_image_free(bytes);
    glBindTexture(texType, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
    // Активируем шейдер и устанавливаем Uniform для сэмплера
    shader.Activate();
    shader.SetUniform1i(uniform, unit); 
}

void Texture::Bind()
{
    glBindTexture(type, ID);
}

void Texture::Unbind()
{
    glBindTexture(type, 0);
}

void Texture::Delete()
{
    glDeleteTextures(1, &ID);
}