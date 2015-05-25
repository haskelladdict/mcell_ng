// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

// TODO: To make filename and path handling more robust it would be better
// to use boost::filesystem.

#ifndef IO_HPP
#define IO_HPP

#include <string>

#include "state.hpp"

// write_cellblender writes the molecule info at iter to a file name in
// cellblender format located at path.
bool write_cellblender(std::string path, std::string name, int iter,
  const VolMols& mols);


#endif
