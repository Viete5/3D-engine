#ifndef VECTOR4_H
#define VECTOR4_H

#include <cmath>

class Vector4 {
public:
    Vector4(float,float,float, float);
    Vector4();
    float getX() const;
    float getY() const;
    float getZ() const;
    float getW() const;
    void setX(float);
    void setY(float);
    void setZ(float);
    void setW(float);
    float dist(float x1=0, float y1=0, float z1=0, float w1=0) const;
    Vector4 operator+(const Vector4&) const;
    Vector4 operator-(const Vector4&) const;
    float operator*(const Vector4&) const;
    Vector4 operator*(float) const;
    float distBetweenDots(const Vector4&) const;
    Vector4 normalize() const;
private:
    float x;
    float y;
    float z;
    float w;
};





#endif //VECTOR4_H