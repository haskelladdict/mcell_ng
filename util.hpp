// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef UTIL_HPP
#define UTIL_HPP

#include <cmath>
#include <ostream>
#include <vector>

// Vector3D represents a 3D vector
struct Vector3D {
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
Vector3D operator+(const Vector3D& a, const Vector3D& b);

// operator+= provides in place vector addition
Vector3D& operator+=(Vector3D& a, const Vector3D& b);

// operator- provides vector subtraction
Vector3D operator-(const Vector3D& a, const Vector3D& b);

// operator-= provides in place vector subtraction
Vector3D& operator-=(Vector3D& a, const Vector3D& b);

// operator<< for Vector3D is intended mostly to help with debugging
std::ostream& operator<<(std::ostream& os, const Vector3D& v);

// operator* implements a dot product between two Vector3Ds
// and a scalar multiplication between a Vector3D and a double
double operator*(const Vector3D& a, const Vector3D& b);
Vector3D operator*(double r, const Vector3D& a);

// cross provides a cross product between two Vector3Ds
Vector3D cross(const Vector3D& a, const Vector3D& b);

// normalize returns a normalized version of the supplied vector
Vector3D normalize(const Vector3D& a);


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
