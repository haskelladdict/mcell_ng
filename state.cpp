// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <algorithm>
#include <cassert>

#include "state.hpp"

// constructor
State::State(uint64_t seed) : rng_{seed} {}


// create_Mesh adds a new mesh object to the simulation
void State::add_mesh(const Mesh& m) {
  mesh_.insert(mesh_.end(), m.begin(), m.end());
}
