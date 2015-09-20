// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <algorithm>
#include <cassert>

#include "state.hpp"

// constructor
State::State(uint64_t seed) : rng_{seed} {}


// add_geometry adds the model geometry to the state. The model geometry is
// defined by a list of tets (which define the topology) and a mesh which
// keeps track of all the triangles making up the tets.
void State::add_geometry(const geom::Mesh& mesh, const geom::Tets& tets) {
  mesh_ = mesh;
  tets_ = tets;
}
