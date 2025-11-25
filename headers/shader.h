#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
class Shader
{
    public:
    // the program ID
    unsigned int ID;
    void SetUniform4f(const char* , float, float, float , float );
    void SetUniform1i(const char*, int);
    // constructor reads and builds the shader
    Shader(const char* , const char*);
    // use/activate the shader
    void Activate();
    void Delete();
};
#endif //SHADER_H