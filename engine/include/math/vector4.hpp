#ifndef VECTOR4_H
#define VECTOR4_H

#include <cmath>

class Vector4 {
public:
    Vector4(float,float,float, float);
    Vector4();
    float get_x() const;
    float get_y() const;
    float get_z() const;
    float get_w() const;
    void set_x(float);
    void set_y(float);
    void set_z(float);
    void set_w(float);
    float dist(float x1=0, float y1=0, float z1=0, float w1=0) const;
    Vector4 operator+(const Vector4&) const;
    Vector4 operator-(const Vector4&) const;
    float operator*(const Vector4&) const;
    Vector4 operator*(float) const;
    float dist_between_dots(const Vector4&) const;
    Vector4 normalize() const;
private:
    float x;
    float y;
    float z;
    float w;
};





#endif //VECTOR4_H