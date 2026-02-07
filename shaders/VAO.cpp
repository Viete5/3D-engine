#include "..\headers\VAO.h"


VAO::VAO() {
    glGenVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO& VBO, unsigned int layout, unsigned int numComponents, unsigned int type, int stride, void* offset) const{
    VBO.Bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    VBO.Unbind();
}

void VAO::Bind() const {
    glBindVertexArray(ID);
}

void VAO::Unbind() const{
    glBindVertexArray(0);
}

void VAO::Delete() {
    glDeleteVertexArrays(1, &ID);
}