#ifndef TORUS_H
#define TORUS_H

#include <vector>
#include <cmath>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include  "Vector.h"
#include  "Matrix.h"

class Torus {
public:
    Torus(float R, float r, int stacks, int slices);
    ~Torus();

    void Draw();
    int getIndexCount() const {return indexCount;}

private:
    VAO torusVAO;
    VBO torusVBO;
    EBO torusEBO;
    int indexCount;

    float R;
    float r;
    float stacks;
    float slices;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    Vector calculatePoint(float, float) const;

    Vector calculateNormal(float, float) const;

    void generateGeometry();

};








#endif TORUS_H