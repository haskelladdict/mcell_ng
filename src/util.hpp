// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef UTIL_HPP
#define UTIL_HPP

#include <cmath>
#include <limits>
#include <ostream>
#include <vector>


// same compares two floating point values for equality
// NOTE: This wrapper is currently not very sophisticated. Needs more work.
inline bool same(double x, double y) {
  return fabs(x - y) < std::numeric_limits<double>::epsilon();
}


// range checked vector implementation
template<typename T>
class Rvector : public std::vector<T> {

public:
  using std::vector<T>::vector;

  T& operator[](int i) {
    return std::vector<T>::at(i);
  }

  const T& operator[](int i) const {
    return std::vector<T>::at(i);
  }
};


// utility typedefs
using SizeTVec = Rvector<size_t>;


// operator<< for Rvector for debugging purposes
template<typename T>
std::ostream& operator<<(std::ostream& os, const Rvector<T>& rvec) {
  os << "[";
  for (const auto& v : rvec) {
    os << v << ",";
  }
  os << "]";
  return os;
}


#endif
