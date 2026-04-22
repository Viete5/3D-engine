#include "../include/math/vector4.hpp"

namespace engine::math {

Vector4::Vector4(float x, float y,float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}
Vector4::Vector4() : Vector4(0,0,0,0) {}

float Vector4::get_x() const{
    return x;
}
float Vector4::get_y() const{
    return y;
}

float Vector4::get_z() const{
    return z;
}

float Vector4::get_w() const{
    return w;
}

void Vector4::set_x(float x) {
    this->x = x;
}
void Vector4::set_y(float y) {
    this->y = y;
}
void Vector4::set_z(float z) {
    this->z = z;
}
void Vector4::set_w(float w) {
    this->w = w;
}

// Distance between dots
float Vector4::dist(float x1,float y1, float z1, float w1) const{
    return sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1)+(z-z1)*(z-z1)+(w-w1)*(w-w1));
}

// Vector sum
Vector4 Vector4::operator+(const Vector4 &rv) const{
    Vector4 tmp(rv.get_x()+x,rv.get_y()+y,rv.get_z()+z, rv.get_w()+w);
    return tmp;
}


Vector4 Vector4::operator-(const Vector4 &rv) const{
    Vector4 tmp(x-rv.get_x(),y-rv.get_y(),z-rv.get_z(),w-rv.get_w());
    return tmp;
}


float Vector4::operator*(const Vector4 &rv) const {
    return rv.get_x()*x+rv.get_y()*y+rv.get_z()*z+rv.get_w()*w;
}
Vector4 Vector4::operator*(float num) const {
    Vector4 temp(num*get_x(),num*get_y(),num*get_z(),num*get_w());
    return temp;
}

float Vector4::dist_between_dots(const Vector4 &rv) const {
    return dist(rv.get_x(),rv.get_y(),rv.get_z(),rv.get_w());
}

Vector4 Vector4::normalize() const{
    float length = dist();
    if (length <= 1e-6f) {
        return Vector4();
    }
    Vector4 temp(get_x()/length,get_y()/length, get_z()/length,get_w()/length);
    return temp;
}

} // namespace engine::math
