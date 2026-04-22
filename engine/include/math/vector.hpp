#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

namespace engine::math {

class Vector {
public:
    Vector(float,float,float);
    Vector();
    float get_x() const;
    float get_y() const;
    float get_z() const;
    void set_x(float);
    void set_y(float);
    void set_z(float);
    float dist(float x1=0, float y1=0, float z1=0) const;
    Vector operator+(const Vector&) const;
    Vector operator-(const Vector&) const;
    float operator*(const Vector&) const;
    Vector operator*(float) const;
    float dist_between_dots(const Vector&) const;
    Vector normalize() const;
    Vector crossprod(const Vector&) const;
private:
    float x;
    float y;
    float z;
};

} // namespace engine::math

#endif //VECTOR_H