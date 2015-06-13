// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef UTIL_HPP
#define UTIL_HPP

#include <ostream>
#include <vector>

// Vector3D represents a 3D vector
struct Vector3D {
  double x;
  double y;
  double z;
};

// operator+ provides vector addition
Vector3D operator+(const Vector3D& a, const Vector3D& b);

// operator+= provides in place vector addition
Vector3D& operator+=(Vector3D& a, const Vector3D& b);

// operator- provides vector subtraction
Vector3D operator-(const Vector3D& a, const Vector3D& b);

// operator-= provides in place vector subtraction
Vector3D& operator-=(Vector3D& a, const Vector3D& b);

// provide operator<< for Vector3D to help with debugging
std::ostream& operator<<(std::ostream& os, const Vector3D& v);


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

#endif
