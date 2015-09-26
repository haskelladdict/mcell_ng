// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>
#include <limits>
#include <ostream>


namespace geom {

// Vec3 represents a 3D vector
struct Vec3 {
  double x = 0.0;
  double y = 0.0;
  double z = 0.0;
};


// same compares two floating point values for equality
// NOTE: This wrapper is currently not very sophisticated. Needs more work.
inline bool same(double x, double y) {
  return fabs(x - y) < std::numeric_limits<double>::epsilon();
}


// squared vector norm
inline double norm2(const Vec3& v) noexcept {
    return v.x*v.x + v.y*v.y + v.z*v.z;
}


// vector norm
inline double norm(const Vec3& v) {
  return sqrt(norm2(v));
}


// operator+ provides vector addition
inline Vec3 operator+(const Vec3& a, const Vec3& b) noexcept {
  return Vec3{a.x + b.x, a.y + b.y, a.z + b.z};
}


// operator+= provides in place vector addition
inline Vec3& operator+=(Vec3& a, const Vec3& b) noexcept {
  a.x += b.x;
  a.y += b.y;
  a.z += b.z;
  return a;
}


// operator- provides vector subtraction
inline Vec3 operator-(const Vec3& a, const Vec3& b) noexcept {
  return Vec3{a.x - b.x, a.y - b.y, a.z - b.z};
}


// operator-= provides in place vector subtraction
inline Vec3& operator-=(Vec3& a, const Vec3& b) noexcept {
  a.x -= b.x;
  a.y -= b.y;
  a.z -= b.z;
  return a;
}


// operator<< for Vec3 is intended mostly to help with debugging
inline std::ostream& operator<<(std::ostream& os, const Vec3& v) {
  return os << "{" << v.x << "," << v.y << "," << v.z << "}";
}


// operator* implements a dot product between two Vec3s
// and a scalar multiplication between a Vec3 and a double
inline double operator*(const Vec3& a, const Vec3& b) noexcept {
  return (a.x*b.x + a.y*b.y + a.z*b.z);
}

inline Vec3 operator*(double r, const Vec3& a) noexcept {
  return Vec3{r * a.x, r * a.y, r * a.z};
}


// operator== provides vector comparison
inline bool operator==(const Vec3& a, const Vec3& b) {
  return same(a.x, b.x) && same(a.y, b.y) && same(a.z, b.z);
}


// cross provides a cross product between two Vec3s
inline Vec3 cross(const Vec3& a, const Vec3& b) noexcept {
  return Vec3{(a.y * b.z) - (a.z * b.y)
             ,(a.z * b.x) - (a.x * b.z)
             ,(a.x * b.y) - (a.y * b.x)};
}


// normalize returns a normalized version of the supplied vector
inline Vec3 normalize(const Vec3& a) {
  return (1 / norm(a)) * a;
}

}

#endif
