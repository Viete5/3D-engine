#include "../include/math/vector.hpp"

namespace engine::math {

Vector::Vector(float x, float y,float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}
Vector::Vector() : Vector(0,0,0) {}

float Vector::get_x() const{
    return x;
}
float Vector::get_y() const{
    return y;
}

float Vector::get_z() const{
    return z;
}

void Vector::set_x(float x) {
    this->x = x;
}
void Vector::set_y(float y) {
    this->y = y;
}
void Vector::set_z(float z) {
    this->z = z;
}

// Distance between dots
float Vector::dist(float x1,float y1, float z1) const{
    return sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1)+(z-z1)*(z-z1));
}

// Vector sum
Vector Vector::operator+(const Vector &rv) const{
    Vector tmp(rv.get_x()+x,rv.get_y()+y,rv.get_z()+z);
    return tmp;
}


Vector Vector::operator-(const Vector &rv) const{
    Vector tmp(x-rv.get_x(),y-rv.get_y(),z-rv.get_z());
    return tmp;
}


float Vector::operator*(const Vector &rv) const {
    return rv.get_x()*x+rv.get_y()*y+rv.get_z()*z;
}
Vector Vector::operator*(float num) const {
    Vector temp(num*get_x(),num*get_y(),num*get_z());
    return temp;
}

float Vector::dist_between_dots(const Vector &rv) const {
    return dist(rv.get_x(),rv.get_y(),rv.get_z());
}

Vector Vector::normalize() const{
    float length = dist();
    if (length <= 1e-6f) {
        return Vector();
    }
    Vector temp(get_x()/length,get_y()/length, get_z()/length);
    return temp;
}

Vector Vector::crossprod(const Vector& other) const {
    float x_new = get_y()*other.get_z()-get_z()*other.get_y();
    float y_new = get_z()*other.get_x()-get_x()*other.get_z();
    float z_new = get_x()*other.get_y()-get_y()*other.get_x();
    Vector temp(x_new,y_new,z_new);
    return temp;
}

} // namespace engine::math
