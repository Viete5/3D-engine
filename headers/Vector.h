#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

class Vector {
public:
    Vector(float,float,float);
    Vector();
    float getX() const;
    float getY() const;
    float getZ() const;
    void setX(float);
    void setY(float);
    void setZ(float);
    float dist(float x1=0, float y1=0, float z1=0) const;
    Vector operator+(const Vector&) const;
    Vector operator-(const Vector&) const;
    float operator*(const Vector&) const;
    Vector operator*(float) const;
    float distBetweenDots(const Vector&) const;
    Vector normalize() const;
    Vector crossprod(const Vector&) const;
private:
    float x;
    float y;
    float z;
};





#endif //VECTOR_H