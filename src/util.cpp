// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <cmath>

#include "util.hpp"


// operator+ provides vector addition
Vec3 operator+(const Vec3& a, const Vec3& b) {
  return Vec3{a.x + b.x, a.y + b.y, a.z + b.z};
}


// operator+= provides in place vector addition
Vec3& operator+=(Vec3& a, const Vec3& b) {
  a.x += b.x;
  a.y += b.y;
  a.z += b.z;
  return a;
}


// operator- provides vector subtraction
Vec3 operator-(const Vec3& a, const Vec3& b) {
  return Vec3{a.x - b.x, a.y - b.y, a.z - b.z};
}


// operator-= provides in place vector subtraction
Vec3& operator-=(Vec3& a, const Vec3& b) {
  a.x -= b.x;
  a.y -= b.y;
  a.z -= b.z;
  return a;
}


// operator<< for Vec3 is intended mostly to help with debugging
std::ostream& operator<<(std::ostream& os, const Vec3& v) {
  return os << "{" << v.x << "," << v.y << "," << v.z << "}";
}


// operator* implements a dot product between two Vec3s
// and a scalar multiplication between a Vec3 and a double
double operator*(const Vec3& a, const Vec3& b) {
  return (a.x*b.x + a.y*b.y + a.z*b.z);
}

Vec3 operator*(double r, const Vec3& a) {
  return Vec3{r * a.x, r * a.y, r * a.z};
}


// cross provides a cross product between two Vec3s
Vec3 cross(const Vec3& a, const Vec3& b) {
  return Vec3{ (a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z),
    (a.x * b.y) - (a.y * b.x)};
}


// normalize returns a normalized version of the supplied vector
Vec3 normalize(const Vec3& a) {
  return (1 / a.norm()) * a;
}
