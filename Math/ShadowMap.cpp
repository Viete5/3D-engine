#include "..\headers\ShadowMap.h"
#include "..\headers\Light.h"

// Конструктор
ShadowMap::ShadowMap(unsigned int width, unsigned int height) : width(width), height(height) {
    // Создаем Framebuffer Object (FBO)
    glGenFramebuffers(1, &FBO);

    // Создаем текстуру для хранения глубины
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    // Настраиваем параметры текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Привязываем текстуру к FBO как буфер глубины
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

    // Отключаем буфер цвета, так как нам нужна только глубина
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // Отвязываем FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Деструктор
ShadowMap::~ShadowMap() {
    glDeleteFramebuffers(1, &FBO);
    glDeleteTextures(1, &depthMap);
}

// Начало рендеринга карты глубины
void ShadowMap::BeginRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glViewport(0, 0, width, height);
    glClear(GL_DEPTH_BUFFER_BIT);
}

// Окончание рендеринга карты глубины
void ShadowMap::EndRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Привязка карты теней к текстурному юниту
void ShadowMap::Bind(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, depthMap);
}

