#include "C:\prog\C++\openGL\headers\Torus.h"

#include <vector>

const float pi = 3.1415926535897932384626433832795028841f;
#include "C:\prog\C++\openGL\headers\Torus.h"
#include <vector>
#include <cmath> 


Torus::Torus(float R, float r, int stacks, int slices) 
    : torusVAO(), 
      torusVBO(), 
      torusEBO(), 
      R(R), 
      r(r), 
      stacks(stacks), 
      slices(slices), 
      indexCount(0)
{

    generateGeometry();
    
    torusVAO.Bind();

    torusVBO.Bind();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    torusEBO.Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    indexCount = indices.size();

    GLsizeiptr stride = 8 * sizeof(float);

    torusVAO.LinkAttrib(torusVBO, 0, 3, GL_FLOAT, stride, (void*)0);
    torusVAO.LinkAttrib(torusVBO, 1, 2, GL_FLOAT, stride, (void*)(3 * sizeof(float)));
    torusVAO.LinkAttrib(torusVBO, 2, 3, GL_FLOAT, stride, (void*)(5 * sizeof(float)));

    torusVAO.Unbind();
    torusVBO.Unbind();
    torusEBO.Unbind();

    
    vertices.clear();
    indices.clear();
}


Vector Torus::calculatePoint(float angleXY, float angleZ) const{
    float x = (R + r*std::cos(angleZ))*std::cos(angleXY);
    float y = (R + r*std::cos(angleZ))*std::sin(angleXY);
    float z = r*std::sin(angleZ);
    Vector temp(x,y,z);
    return temp;
}

Vector Torus::calculateNormal(float angleXY,float angleZ) const {
    float x = std::cos(angleZ)*std::cos(angleXY);
    float y = std::cos(angleZ)*std::sin(angleXY);
    float z = std::sin(angleZ);
    Vector temp(x,y,z);
    return temp;
} 

void Torus::generateGeometry() {
    float dU = 2.0*pi/slices;
    float dV = 2.0*pi/stacks;
    for (int i = 0;i<=stacks;i++) {
        for (int j = 0;j<=slices;j++) {

            float u1 = j*dU;
            float v1 = i*dV;

            Vector V1 = calculatePoint(u1,v1);
            Vector V1Nor = calculateNormal(u1,v1);

            float Utex = (float)j/slices;
            float Vtex = (float)i/stacks;

            vertices.push_back(V1.getX());
            vertices.push_back(V1.getY());
            vertices.push_back(V1.getZ());
            vertices.push_back(Utex);
            vertices.push_back(Vtex);
            vertices.push_back(V1Nor.getX());
            vertices.push_back(V1Nor.getY());
            vertices.push_back(V1Nor.getZ());
        }
    }
    for (int i=0;i<stacks;++i) {
        for (int j=0;j<slices;++j) {

            int BaseIndex = i*(slices+1)+j;
            unsigned int P_A = BaseIndex;
            unsigned int P_B = BaseIndex + 1;  
            unsigned int P_C = BaseIndex + (slices + 1);  
            unsigned int P_D = BaseIndex + (slices + 1) + 1;

            indices.push_back(P_A);
            indices.push_back(P_B);
            indices.push_back(P_C);

            indices.push_back(P_D);
            indices.push_back(P_C);
            indices.push_back(P_B);
        }
    }
}

void Torus::Draw()  {
    torusVAO.Bind();
    glDrawElements(GL_TRIANGLES,indexCount,GL_UNSIGNED_INT,0);
    torusVAO.Unbind();
}

Torus::~Torus() {
    torusVAO.Delete();
    torusVBO.Delete();
    torusEBO.Delete();
}