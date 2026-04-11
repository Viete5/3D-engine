#ifndef CUBE_H
#define CUBE_H

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Vector.h"
#include "Matrix.h"
#include "shader.h"
#include "Object.h"

class Cube : public Object{
public:
    Cube(); 
    void Delete();
    ~Cube();
private:
    VAO cubeVAO;
    VBO cubeVBO;
    EBO cubeEBO;
};





#endif //CUBE_H