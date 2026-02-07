#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include "Matrix.h"
#include "Light.h"

#include <glad/glad.h>

class ShadowMap {
public:
    unsigned int width;
    unsigned int height; 
    ShadowMap(unsigned int width = 1024, unsigned int height = 1024);
    ~ShadowMap();

   // Render of depthMap
    void BeginRender() const;
    void EndRender() const;

    // Bind to texture
    void Bind(GLenum textureUnit) const;

    
private:
    unsigned int FBO;          
    unsigned int depthMap;                           
};

#endif // SHADOW_MAP_H