#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include "Vector.hpp"

class Matrix4 {
public:
    Matrix4();
    Matrix4(float);
    float& at(int, int);
    const float& at(int, int) const;
    Matrix4 operator*(const Matrix4 &) const;
    Matrix4 get_trans();
    float det();
    Matrix4 reversed();
    static Matrix4 get_translate(float, float,float);
    static Matrix4 get_rotate_x(float);
    static Matrix4 get_rotate_y(float);
    static Matrix4 get_rotate_z(float);
    static Matrix4 get_scale(float,float,float);
    static Matrix4 get_perspective(float , float , float, float);
    static Matrix4 get_look_at(const Vector&,const Vector&,const Vector&);
    static Matrix4 get_ortho(float,float,float,float,float,float);
    Matrix4 get_norm_matrix() const;

private:
    Matrix4 get_trian_matrix(int&);
    int choice_leading(Matrix4 &,int,int*);
    static const int dim=4;
    float m[16];  
};





#endif //MATRIX_H