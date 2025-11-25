#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include <C:\prog\C++\openGL\headers\Vector.h>

class Matrix4 {
public:
    Matrix4();
    Matrix4(float);
    void setM(float*);
    float& at(int, int);
    const float& at(int, int) const;
    Matrix4 operator*(const Matrix4 &) const;
    Matrix4 trans();
    float det();
    Matrix4 reversed();
    static Matrix4 translate(float, float,float);
    static Matrix4 rotateX(float);
    static Matrix4 rotateY(float);
    static Matrix4 rotateZ(float);
    static Matrix4 scale(float,float,float);
    static Matrix4 Perspective(float , float , float, float);
    static Matrix4 lookAt(const Vector&,const Vector&,const Vector&);

private:
    Matrix4 trianM(int&);
    int choiceLeading(Matrix4 &,int,int*);
    static const int dim=4;
    float m[16];  
};





#endif //MATRIX_H