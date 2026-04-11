#include "..\headers\Cube.h"


// Settings of basic cube
namespace {
  float vertices[] = {
    // Front
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        // Back
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        // Up
        0.5f, 0.5f, 0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, 0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        // Down
        0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f,  -0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        -0.5f,  -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        -0.5f,  -0.5f,  -0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        // Right
        0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f,  0.5f, 0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        // Left
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
  };
  unsigned int indices[] = {
        0, 1, 2,  2, 3, 0,
        4, 7, 6,  6, 5, 4,
        8, 9, 10,  9, 11, 10,
        14, 13, 12,  14, 15, 13, 
        16, 17, 18,  19, 18, 17,
        20, 22, 23,  23, 21, 20
    };
}


Cube::Cube() 
    : cubeVBO(vertices,sizeof(vertices)),
      cubeEBO(indices, sizeof(indices)),
      Object(0,36)
{      
  // Binding       
  cubeVAO.Bind();
  cubeEBO.Bind();

  // Linking
  // Position
  cubeVAO.LinkAttrib(cubeVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
  // Texture
  cubeVAO.LinkAttrib(cubeVBO, 1, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  // Normals
  cubeVAO.LinkAttrib(cubeVBO, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));
  
  ObjVAO_ID = cubeVAO.ID;

  // Unbinding
  cubeVAO.Unbind();
  cubeVBO.Unbind();
  cubeEBO.Unbind();
}

void Cube::Delete() {
  cubeVAO.Delete();
  cubeEBO.Delete();
  cubeVBO.Delete();
}


Cube::~Cube() {
  Delete();
}