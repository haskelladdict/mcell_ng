// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <cmath>
#include <limits>

#include "util.hpp"


// operator+ provides vector addition
Vector3D operator+(const Vector3D& a, const Vector3D& b) {
  return Vector3D{a.x + b.x, a.y + b.y, a.z + b.z};
}


// operator+= provides in place vector addition
Vector3D& operator+=(Vector3D& a, const Vector3D& b) {
  a.x += b.x;
  a.y += b.y;
  a.z += b.z;
  return a;
}


// operator- provides vector subtraction
Vector3D operator-(const Vector3D& a, const Vector3D& b) {
  return Vector3D{a.x - b.x, a.y - b.y, a.z - b.z};
}


// operator-= provides in place vector subtraction
Vector3D& operator-=(Vector3D& a, const Vector3D& b) {
  a.x -= b.x;
  a.y -= b.y;
  a.z -= b.z;
  return a;
}


// operator<< for Vector3D is intended mostly to help with debugging
std::ostream& operator<<(std::ostream& os, const Vector3D& v) {
  return os << "{" << v.x << "," << v.y << "," << v.z << "}";
}


// operator* implements a dot product between two Vector3Ds
// and a scalar multiplication between a Vector3D and a double
double operator*(const Vector3D& a, const Vector3D& b) {
  return (a.x*b.x + a.y*b.y + a.z*b.z);
}

Vector3D operator*(double r, const Vector3D& a) {
  return Vector3D{r * a.x, r * a.y, r * a.z};
}


// cross provides a cross product between two Vector3Ds
Vector3D cross(const Vector3D& a, const Vector3D& b) {
  return Vector3D{ (a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z),
    (a.x * b.y) - (a.y * b.x)};
}


// normalize returns a normalized version of the supplied vector
Vector3D normalize(const Vector3D& a) {
  return (1 / a.norm()) * a;
}


// same compares two floating point values for equality
// NOTE: This wrapper is currently not very sophisticated. Needs more work.
bool same(double x, double y) {
  return fabs(x - y) < std::numeric_limits<double>::epsilon();
}




