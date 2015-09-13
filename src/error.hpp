// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>

// Error is a small helper struct for returning errors. It consists of a bool
// indicating that an error occured as well as a string describing the error.
struct Error {
  explicit Error(std::string d, bool e = true) : err(e), desc(d) {};

  bool err;
  std::string desc;
};

// noErr indicated that no error has occured
const Error noErr{"", false};

#endif
