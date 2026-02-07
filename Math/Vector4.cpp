#include "..\headers\Vector4.h"

Vector4::Vector4(float x, float y,float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}
Vector4::Vector4() : Vector4(0,0,0,0) {}

float Vector4::getX() const{
    return x;
}
float Vector4::getY() const{
    return y;
}

float Vector4::getZ() const{
    return z;
}

float Vector4::getW() const{
    return w;
}

void Vector4::setX(float x) {
    this->x = x;
}
void Vector4::setY(float y) {
    this->y = y;
}
void Vector4::setZ(float z) {
    this->z = z;
}
void Vector4::setW(float w) {
    this->w = w;
}

// Distance between dots
float Vector4::dist(float x1,float y1, float z1, float w1) const{
    return sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1)+(z-z1)*(z-z1)+(w-w1)*(w-w1));
}

// Vector sum
Vector4 Vector4::operator+(const Vector4 &rv) const{
    Vector4 tmp(rv.getX()+x,rv.getY()+y,rv.getZ()+z, rv.getW()+w);
    return tmp;
}


Vector4 Vector4::operator-(const Vector4 &rv) const{
    Vector4 tmp(x-rv.getX(),y-rv.getY(),z-rv.getZ(),w-rv.getW());
    return tmp;
}


float Vector4::operator*(const Vector4 &rv) const {
    return rv.getX()*x+rv.getY()*y+rv.getZ()*z+rv.getW()*w;
}
Vector4 Vector4::operator*(float num) const {
    Vector4 temp(num*getX(),num*getY(),num*getZ(),num*getW());
    return temp;
}

float Vector4::distBetweenDots(const Vector4 &rv) const {
    return dist(rv.getX(),rv.getY(),rv.getZ(),rv.getW());
}

Vector4 Vector4::normalize() const{
    float length = dist();
    Vector4 temp(getX()/length,getY()/length, getZ()/length,getW()/length);
    return temp;
}
