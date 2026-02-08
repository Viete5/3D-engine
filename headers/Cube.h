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
    // void draw(Shader& shader) const; 
    void Delete();
    // void setPosition(Vector newPos) { pos = newPos; } // Change the position of cube
    // void SetRotation(float x, float y); // Rotate cube
    ~Cube();
private:
    VAO cubeVAO;
    VBO cubeVBO;
    EBO cubeEBO;

    // float rotX = 0.0f;
    // float rotY = 0.0f;
    
    // Position of the centre
    // Vector pos;
};





#endif //CUBE_H