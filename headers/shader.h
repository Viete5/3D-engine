#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Matrix.h"
#include "Vector.h"


class Shader
{
    public:
    // the program ID
    unsigned int ID;
    void SetUniform4f(const char* , float, float, float , float );
    void SetUniform1i(const char*, int);

    // constructor reads and builds the shader
    Shader(const char* , const char*);

    // Uniforms
    void setMat4(const std::string& name, const Matrix4& mat) const;
    void setVec3(const std::string& name, const Vector& vec) const;
    void setInt(const std::string& name, int value) const;

    // use/activate the shader
    void Activate();
    void Delete();
};
#endif //SHADER_H