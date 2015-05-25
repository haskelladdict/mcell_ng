// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef UTIL_HPP
#define UTIL_HPP

#include <ostream>
#include <vector>

// vector3D represents a 3D vector
struct vector3D {
  double x;
  double y;
  double z;
};

// operator+ provides vector addition
vector3D operator+(const vector3D& a, const vector3D& b);

// operator+= provides in place vector addition
vector3D& operator+=(vector3D& a, const vector3D& b);

// provide operator<< for vector3D to help with debugging
std::ostream& operator<<(std::ostream& os, const vector3D& v);


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
