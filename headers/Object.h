#ifndef OBJECT_H
#define OBJECT_H

#include "VAO.h"
#include "Vector.h"
#include "Texture.h"
#include "shader.h"
#include "Matrix.h"
#include "Vector4.h"

class Object {
public:
    Object(unsigned int  SubObjVBO_ID, unsigned int SubObjVAO_ID, unsigned int SubObjEBO_ID, unsigned int SubObjIndexCount) 
    : ObjVAO_ID(SubObjVAO_ID),
      Position(Vector(0.0f,0.0f,0.0f)),
      Rotation(Vector(0.0f,0.0f,0.0f)),
      Scale(Vector(1.0f,1.0f,1.0f)),
      BaseColor(Vector4(1.0f,1.0f,1.0f,1.0f)),
      ObjTex(nullptr),
      IndexCount(SubObjIndexCount),
      ModelFlag(false)
      {}

    void Draw(Shader& shader);

    void setPosition(Vector newPos);
    void SetRotation(Vector newRot);
    void SetScale(Vector newScl);

    void SetBaseColor(Vector4 newBaseColor) { BaseColor = newBaseColor; }
    void SetObjTex(Texture* newObjTex) { ObjTex = newObjTex; }

    virtual ~Object() = 0;

private:
    Vector Position;
    Vector Rotation;
    Vector Scale;
    Matrix4 Model;
    Matrix4 NormalMatrix;
    void updateMatrix();
    bool ModelFlag;

    unsigned int ObjVAO_ID;

    Vector4 BaseColor;
    Texture* ObjTex;

    unsigned int IndexCount;

    static Texture* WhiteTexture;
};







#endif //OBJECT_H