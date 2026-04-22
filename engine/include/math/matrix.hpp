#ifndef MATRIX_H
#define MATRIX_H

#include <cassert>
#include <cmath>
#include <stdexcept>

namespace engine::math {

class Vector;

class Matrix4 {
public:
    Matrix4();
    Matrix4(float);
    float& at(int, int);
    const float& at(int, int) const;
    Matrix4 operator*(const Matrix4 &) const;
    Matrix4 get_trans() const;
    float det() const;
    Matrix4 inversed() const;
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
    Matrix4 get_trian_matrix(int&) const;
    int choice_leading(int, int&, Matrix4* = nullptr);
    static const int dim=4;
    float m[16];  
};

} // namespace engine::math

#endif //MATRIX_H
