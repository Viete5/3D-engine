#include "../headers/Object.h"

Texture* Object::WhiteTexture = nullptr;

void Object::updateMatrix() {
    // Matrix of trans
    Matrix4 trans = Matrix4::translate(Position.getX(), Position.getY(), Position.getZ());
  
    // Matrix of rotation
    Matrix4 rotationX = Matrix4::rotateX(Rotation.getX());
    Matrix4 rotationY = Matrix4::rotateY(Rotation.getY());
    Matrix4 rotationZ = Matrix4::rotateZ(Rotation.getZ());

    // Matrix of scaling
    Matrix4 scale = Matrix4::scale(Scale.getX(), Scale.getY(), Scale.getZ());

    // Model
    Model = trans * (rotationY * rotationX * rotationZ) * scale;

    // Matrix of normals
    NormalMatrix = Model.normMatrix();
}

void Object::SetPosition(Vector newPos) {
    Position = newPos;
    ModelFlag = true;
}

void Object::SetRotation(Vector newRot) {
    Rotation = newRot;
    ModelFlag = true;
}

void Object::SetScale(Vector newScl) {
    Scale = newScl;
    ModelFlag = true;
}


void Object::Draw(Shader& shader) {
    // Check the update
    if (ModelFlag) {
        updateMatrix();
        ModelFlag = false;
    }

    shader.Activate();
    shader.setMat4("model", Model);
    shader.setMat4("normalMatrix", NormalMatrix);
    shader.setVec4("ObjectColor", BaseColor);
    
    // Check what Texture to bind
    Texture* currentTex = ObjTex;

    if (!currentTex) {
        if (!WhiteTexture) {
            WhiteTexture = new Texture("../resource/white.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
        }
        currentTex = WhiteTexture;
    }

    currentTex->texUnit(shader, "ourTexture", 0);
    currentTex->Bind();


    glBindVertexArray(ObjVAO_ID);
    glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Object::~Object() {}