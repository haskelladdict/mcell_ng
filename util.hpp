// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef UTIL_HPP
#define UTIL_HPP

#include <cmath>
#include <ostream>
#include <vector>

// Vec3 represents a 3D vector
struct Vec3 {
  double x;
  double y;
  double z;

  // squared vector norm
  double norm2() const {
    return x*x + y*y + z*z;
  }

  // vector norm
  double norm() const {
    return sqrt(norm2());
  }
};

// operator+ provides vector addition
Vec3 operator+(const Vec3& a, const Vec3& b);

// operator+= provides in place vector addition
Vec3& operator+=(Vec3& a, const Vec3& b);

// operator- provides vector subtraction
Vec3 operator-(const Vec3& a, const Vec3& b);

// operator-= provides in place vector subtraction
Vec3& operator-=(Vec3& a, const Vec3& b);

// operator<< for Vec3 is intended mostly to help with debugging
std::ostream& operator<<(std::ostream& os, const Vec3& v);

// operator* implements a dot product between two Vec3s
// and a scalar multiplication between a Vec3 and a double
double operator*(const Vec3& a, const Vec3& b);
Vec3 operator*(double r, const Vec3& a);

// cross provides a cross product between two Vec3s
Vec3 cross(const Vec3& a, const Vec3& b);

// normalize returns a normalized version of the supplied vector
Vec3 normalize(const Vec3& a);


// range checked vector implementation
template<typename T>
class Vec : public std::vector<T> {

public:
  using std::vector<T>::vector;

  T& operator[](int i) {
    return std::vector<T>::at(i);
  }

  const T& operator[](int i) const {
    return std::vector<T>::at(i);
  }
};


// same compares two floating point values for equality
bool same(double x, double y);


#endif
