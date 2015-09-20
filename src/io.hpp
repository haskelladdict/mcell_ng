// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

// TODO: To make filename and path handling more robust it would be better
// to use boost::filesystem.

#ifndef IO_HPP
#define IO_HPP

#include <string>

#include "error.hpp"
#include "state.hpp"

// write_cellblender writes the molecule info at iter to a file name in
// cellblender format located at path.
Error write_cellblender(State& state, std::string path, std::string name,
  int iter);


// parse_mcsf_tet_mesh parses an MCSF file containing a tet mesh and creates
// and returns an internal representation of the mesh.
std::tuple<geom::Mesh, geom::Tets, Error> parse_mcsf_tet_mesh(const std::string& fileName);

#endif
