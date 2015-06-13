// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

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


// provide operator<< for Vector3D to help with debugging
std::ostream& operator<<(std::ostream& os, const Vector3D& v) {
  return os << "{" << v.x << "," << v.y << "," << v.z << "}";
}
