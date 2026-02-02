#ifndef CUBE_H
#define CUBE_H

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

class Cube {
public:
    Cube();
    void draw();
    void Delete();
    ~Cube();
private:
    VAO cubeVAO;
    VBO cubeVBO;
    EBO cubeEBO;
};





#endif //CUBE_H