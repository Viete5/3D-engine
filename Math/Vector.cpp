#include "C:\prog\C++\openGL\headers\Vector.h"

Vector::Vector(float x, float y,float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}
Vector::Vector() : Vector(0,0,0) {}

float Vector::getX() const{
    return x;
}
float Vector::getY() const{
    return y;
}

float Vector::getZ() const{
    return z;
}

void Vector::setX(float x) {
    this->x = x;
}
void Vector::setY(float y) {
    this->y = y;
}
void Vector::setZ(float z) {
    this->z = z;
}

//расстояние между точками
float Vector::dist(float x1,float y1, float z1) const{
    return sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1)+(z-z1)*(z-z1));
}

//vector sum
Vector Vector::operator+(const Vector &rv) const{
    Vector tmp(rv.getX()+x,rv.getY()+y,rv.getZ()+z);
    return tmp;
}


Vector Vector::operator-(const Vector &rv) const{
    Vector tmp(x-rv.getX(),y-rv.getY(),z-rv.getZ());
    return tmp;
}


float Vector::operator*(const Vector &rv) const {
    return rv.getX()*x+rv.getY()*y+rv.getZ()*z;
}
Vector Vector::operator*(float num) const {
    Vector temp(num*getX(),num*getY(),num*getZ());
    return temp;
}

float Vector::distBetweenDots(const Vector &rv) const {
    return dist(rv.getX(),rv.getY(),rv.getZ());
}

Vector Vector::normalize() const{
    float length = dist();
    Vector temp(getX()/length,getY()/length, getZ()/length);
    return temp;
}

Vector Vector::crossprod(const Vector& other) const {
    float xNew = getY()*other.getZ()-getZ()*other.getY();
    float yNew = getZ()*other.getX()-getX()*other.getZ();
    float zNew = getX()*other.getY()-getY()*other.getX();
    Vector temp(xNew,yNew,zNew);
    return temp;
}