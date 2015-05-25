// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include "util.hpp"


// operator+ provides vector addition
vector3D operator+(const vector3D& a, const vector3D& b) {
  return vector3D{a.x + b.x, a.y + b.y, a.z + b.z};
}


// operator+= provides in place vector addition
vector3D& operator+=(vector3D& a, const vector3D& b) {
  a.x += b.x;
  a.y += b.y;
  a.z += b.z;
  return a;
}


// provide operator<< for vector3D to help with debugging
std::ostream& operator<<(std::ostream& os, const vector3D& v) {
  return os << "{" << v.x << "," << v.y << "," << v.z << "}";
}
